#include "display.h"
#include <iostream>
#include <QMouseEvent>
#include "mainwindow.h"
#include <cmath>
#include "algorithms.h"
#include <Vector>
#include <QTimer>

using namespace std;

static const GLfloat fov=45.0f,nearPoint=0.1f, farPoint=200.0f;//nearPoint =0.0f will fail it :(
static const GLfloat strokeWidth=1.1f, lineDistance=-0.1f;
static const GLuint sliceInCircle=70, BUFFER_SIZE=2048;
static const GLfloat zoomInterval=0.2f, panelMoveInterval=0.05f,panelDistance=-30.0f;
static const GLfloat freehandWidth=3.0f, SCANSIZE=18.0f;

Display::Display(QWidget *parent, int mode):QGLWidget(parent){
    this->mode=mode;
    setMouseTracking(true);
    leftMousePressed=false;
    rightMousePressed=false;
    drawBase=true;
    panelMoving=false;
    rotatingX=false;
    extrudedShape=0;
    pScene=new Scene();
    pCurrentStrokes=0;
    pLastStrokes=0;
    pDragFrom=0;
    pExtrudeFrom=0;
    drawPanelEnabled=true;
    initializeValues();
}//Display

void Display::initializeValues(){
    drawPanelZoom=4.5f; //half of length in scene coordinates
    drawTranslateX=0.0f;
    drawTranslateY=0.0f;
    drawTranslateZ=0.0f;
    drawRotateX=0;
    drawRotateY=0;
    originalRotateX=0;
    originalRotateY=0;
    selectedObject=-1;
    updateGL();
}

Display::~Display(){
    delete pScene; pScene=0;
    if(pCurrentStrokes) delete pCurrentStrokes; pCurrentStrokes=0;
    if(pLastStrokes) delete pLastStrokes; pLastStrokes=0;
    gluDeleteQuadric(quadric);
}//~Display

void Display::paintGL(){
    glClear (GL_COLOR_BUFFER_BIT);
    /*********right window*****************/
    glEnable(GL_DEPTH_TEST);
    glViewport((GLsizei)(width/3.0f+1.0f), 0, (GLsizei)(width/3.0f*2.0f-1),(GLsizei)height);

    glMatrixMode(GL_PROJECTION);	// Select The Projection Matrix
        glLoadIdentity();// Reset The Projection Matrix
    if(width/3.0f*2.0f-1>height){
        GLfloat temp=(GLfloat)height/(GLfloat)width*3.0f/2.0f;
        panelHalfSpanW=drawPanelZoom;
        panelHalfSpanH=drawPanelZoom*temp;
    }
    else{
        GLfloat temp=(GLfloat)width/(GLfloat)height/3.0f*2.0f;
        panelHalfSpanW=drawPanelZoom*temp;
        panelHalfSpanH=drawPanelZoom;
    }
    glOrtho(-1.0f*panelHalfSpanW,panelHalfSpanW,-1.0*panelHalfSpanH,panelHalfSpanH,nearPoint,farPoint);

    glMatrixMode (GL_MODELVIEW);			// Select The Modelview Matrix
    glLoadIdentity ();						// Reset The Modelview Matrix
    glClear (GL_DEPTH_BUFFER_BIT);
    drawScene(true);

    /************Left window***************/
    glViewport(0, 0, (GLsizei)(width/3.0f-1), (GLsizei)height);
    glMatrixMode(GL_PROJECTION);	// Select The Projection Matrix
        glLoadIdentity();// Reset The Projection Matrix
    gluPerspective(fov,((GLfloat)width-1)/(GLfloat)height/3.0,nearPoint,farPoint);

    glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
        glLoadIdentity();
    glClear (GL_DEPTH_BUFFER_BIT);
    drawScene(false);
    drawMidLine();
    drawText();
}//paintGL (repaint)

void Display::drawScene(bool drawPanel){
    glTranslatef(1.0f*drawTranslateX,1.0f*drawTranslateY,0);
    glColor3f(0.0,.0,0.5);
    if(drawPanelEnabled&&drawPanel){
        glLineWidth(strokeWidth);
        glPushMatrix();
        if(panelMoving) glRotatef(50.0f,0,1,0);
        glBegin(GL_LINES);
            glVertex3f(0,100.0f*panelHalfSpanH,panelDistance+drawTranslateZ+0.01f);
            glVertex3f(0,-100.0f*panelHalfSpanH,panelDistance+drawTranslateZ+0.01f);
            glVertex3f(-100.0f*panelHalfSpanW,0,panelDistance+drawTranslateZ+0.01f);
            glVertex3f(100.0f*panelHalfSpanW,0,panelDistance+drawTranslateZ+0.01f);
        glEnd();
        glPopMatrix();
    }
    glTranslatef(0,0,panelDistance+0.01f); //definitely last stage
    if(panelMoving&&drawPanel) glRotatef(50.0f,0,1,0);

    /*origin=Vec3f(0.0f,0.0f,0.0f);
    vec=Vec3f(drawRotateX,drawRotateY,0.0f);
    Vec3f direction=vec-origin;
    float angle=direction.Length();
    glRotatef(angle,direction.x,direction.y,0.0f);*/

    glRotatef(drawRotateY,0.0,1.0f,0);
    glRotatef(drawRotateX,1.0f,0,0);

    if(drawPanel){
        glPushMatrix();
        glColor3f(0.0,0.0,0.0);
        drawStrokes(pCurrentStrokes);
        //glColor3f(0.0,0.0,0.0);
        glColor3f(0.6,0.6,0.6);
        drawStrokes(pLastStrokes);
        glPopMatrix();
    }
    vector<Object*> sceneObjects=pScene->getAll();
    for(int i=sceneObjects.size()-1;i>=0;i--){
        glPushMatrix();
        Object * temp=sceneObjects[i];
        glLoadName(i);//needed for picking
        switch(temp->getType()){
            case(Object::CIRCLE):
                if(i==selectedObject) glColor4f(1.0,0.4,0.4,1.0);
                else glColor4f(0.9,0.0,0.0,1.0);
                drawCircle(static_cast<Circle*>(temp));
            break;
            case(Object::TRIANGLE):
                 if(i==selectedObject) glColor4f(0.3,0.9,0.3,1.0);
                 else glColor4f(0,0.7,0,1.0);
                drawTriangle(static_cast<Triangle*>(temp));
            break;
            case(Object::FREEHAND):
                if(i==selectedObject) glColor4f(0.1,0.1,0.8,1.0);
                else glColor4f(0,0.0,0.4,1.0);
                drawFreehand(static_cast<Freehand*>(temp));
            break;
            case(Object::SPHERE):
                if(i==selectedObject) glColor4f(0.6,0.6,0.9,1.0);
                else glColor4f(0.5,0.5,0.8,1.0);
                drawSphere(static_cast<Sphere*>(temp));
            break;
            case(Object::RECTANGLE):
                if(i==selectedObject) glColor4f(0.95,0.95,0.2,1.0);
                else glColor4f(0.85,0.85,0.2,1.0);
                drawRectangle(static_cast<Rect*>(temp));
            break;
            case(Object::CYLINDER):
                if(i==selectedObject) glColor4f(1.0f,0.7,0.1,1.0);
                else glColor4f(1.0f,0.5,0.1,1.0);
                drawCylinder(static_cast<Cylinder*>(temp));
            break;
            case(Object::CONE):
                if(i==selectedObject) glColor4f(0.8f,0.0,0.8,1.0);
                else glColor4f(0.7,0,0.7,1.0);
                drawCone(static_cast<Cone*>(temp));
            break;

            default: break;

        }//switch
        glPopMatrix();
    }//for (go through vector of scene objects
    if(drawPanelEnabled&&drawPanel&&mode!=MainWindow::mode_delete) drawDrawPanel();
}//drawScene

void Display::mousePressEvent(QMouseEvent *event){
    statusString="";
    //cout<<"mouse press"<<endl;
    if(!rightMousePressed&&event->button()==Qt::LeftButton){

        if(isDrawMode()&&this->isInDrawPanel()){
            leftMousePressed=true;
            Point* p=new Point(windowToSceneW(event->x()),windowToSceneH(event->y()),drawTranslateZ);
            Line * line=new Line(p,drawRotateX,drawRotateY,0);
            if(!pCurrentStrokes)
                pCurrentStrokes=new Strokes(line);
            else pCurrentStrokes->addLine(line);
        }
        else if(mode==MainWindow::mode_delete){
            int result;
            if((result=selection())>-1){
                pScene->removeObject(result);
                selectedObject=-1;
                updateGL();
            }
        }
        else if(mode==MainWindow::mode_push_pull){
            leftMousePressed=true;
            int result;
            if((result=selection())>-1){
               selectedObject=result;
               updateGL();
               extrudedShape=pScene->getShape(result);
               pExtrudeFrom=new Point(windowToSceneW(event->x()),windowToSceneH(event->y()),0);
           }
        }
    }//left mouse
    if(!leftMousePressed&&event->button()==Qt::RightButton){
        if(pCurrentStrokes){
            shapeDetection(true);//user triggered
        }
        else if(isInDrawPanel()){
            setCursor(Qt::ClosedHandCursor);
            rightMousePressed=true;
            pDragFrom=new Point(windowToSceneW(event->x()),windowToSceneH(event->y()),0.0f);//z is panel depth
        }
    }//right mouse
    else if(event->button()==Qt::MidButton){
    }
}//mouse press event

void Display::mouseMoveEvent(QMouseEvent *event){
    mouseX=event->x();
    mouseY=event->y();
    if(mode==MainWindow::mode_delete||mode==MainWindow::mode_push_pull){
        selectedObject=selection();
    }
    if(leftMousePressed){
        if(isDrawMode()&&pCurrentStrokes){
            Point* p=new Point(windowToSceneW(event->x()),windowToSceneH(event->y()),drawTranslateZ);
            pCurrentStrokes->getTailLine()->addPoint(p);
            //cout<<p->toString()<<"\n"<<event->x()<<" "<<event->y()<<endl;
            updateGL();
        }
        else if(mode==MainWindow::mode_push_pull&&extrudedShape){
            GLfloat height=extrudedShape->getHeight();
            height+=(windowToSceneW(event->x())-pExtrudeFrom->getX());
            if (height<0.0f) height=0.0f;
            extrudedShape->setHeight(height);
            updateGL();
            delete pExtrudeFrom; pExtrudeFrom=0;
            pExtrudeFrom=new Point(windowToSceneW(event->x()),windowToSceneH(event->y()),0);
        }
    }//leftMousePressed
    else if(rightMousePressed){
        drawTranslateX+=windowToSceneW(event->x())-pDragFrom->getX();
        drawTranslateY+=windowToSceneH(event->y())-pDragFrom->getY();
        updateGL();
    }
    else if(!isInDrawPanel()&&isDrawMode()){
        shapeDetection(true);
    }
    else if(!panelMoving){
        updateGL();
    }
}//mouseMoveEvent

void Display::shapeDetection(bool userTriggered){
    if(pCurrentStrokes&&pCurrentStrokes->getTailLine()->getTotalPoints()>2){
        switch(mode){
            case MainWindow::mode_cone:
                if(pCurrentStrokes->getTotalLines()==3||userTriggered&&pCurrentStrokes->getTotalLines()==2||userTriggered&&pCurrentStrokes->getTotalLines()==1){
                    delete pLastStrokes;
                    pLastStrokes=pCurrentStrokes;
                    pScene->addObject(Algorithms::coneDetection(pCurrentStrokes,-1.0f*drawRotateX,-1.0f*drawRotateY,0));
                    pCurrentStrokes=0;
                }
            break;

            case MainWindow::mode_cube:
            break;

            case MainWindow::mode_cylinder_drum:
                if(pCurrentStrokes->getTotalLines()==2){
                    delete pLastStrokes;
                    pLastStrokes=pCurrentStrokes;
                    pScene->addObject(Algorithms::cylinderDetection(pCurrentStrokes,false,drawBase,-1.0f*drawRotateX,-1.0f*drawRotateY,0));
                    pCurrentStrokes=0;
                }
            break;

            case MainWindow::mode_cylinder_tube:
                if(pCurrentStrokes->getTotalLines()==2){
                    delete pLastStrokes;
                    pLastStrokes=pCurrentStrokes;
                    pScene->addObject(Algorithms::cylinderDetection(pCurrentStrokes,true,drawBase,-1.0f*drawRotateX,-1.0f*drawRotateY,0));
                    pCurrentStrokes=0;
                }
            break;

            case MainWindow::mode_freehand:
                delete pLastStrokes;
                pLastStrokes=pCurrentStrokes;
                pScene->addObject(Algorithms::freehandDetection(pCurrentStrokes,-1.0f*drawRotateX,-1.0f*drawRotateY,0));
                pCurrentStrokes=0;
            break;

            case MainWindow::mode_sphere:
                delete pLastStrokes;
                pLastStrokes=pCurrentStrokes;
                pScene->addObject(Algorithms::sphereDetection(pCurrentStrokes,-1.0f*drawRotateX,-1.0f*drawRotateY,0));
                pCurrentStrokes=0;
            break;

            case MainWindow::mode_rectangle:
                if(pCurrentStrokes->getTotalLines()==2){
                    delete pLastStrokes;
                    pLastStrokes=pCurrentStrokes;
                    pScene->addObject(Algorithms::rectangleDetection(pCurrentStrokes,-1.0f*drawRotateX,-1.0f*drawRotateY,0));
                    pCurrentStrokes=0;
                 }
            break;

            case MainWindow::mode_triangle:
            if(pCurrentStrokes->getTotalLines()==3||userTriggered&&pCurrentStrokes->getTotalLines()==2||userTriggered&&pCurrentStrokes->getTotalLines()==1){
                delete pLastStrokes;
                pLastStrokes=pCurrentStrokes;
                Triangle * pTriangle=Algorithms::triangleDetection(pCurrentStrokes,-1.0f*drawRotateX,-1.0f*drawRotateY,0);
                pScene->addObject(pTriangle);
                pCurrentStrokes=0;
            }
            break;

            case MainWindow::mode_circle:
                delete pLastStrokes;
                pLastStrokes=pCurrentStrokes;
                //pLastStrokes=new Strokes(Algorithms::angularTolerance(pCurrentStrokes->getTailLine(),triangleTolerance));
                Circle * pCircle=Algorithms::circleDetection(pCurrentStrokes,-1.0f*drawRotateX,-1.0f*drawRotateY,0);//opposite to undo rotation
                //cout<<pCircle->toString()<<endl;
                pScene->addObject(pCircle);
                pCurrentStrokes=0;
            break;

            case MainWindow::mode_push_pull:

            break;
            default: break;
        }//switch
    }//useful line
    else{
        if(pCurrentStrokes)
            delete pCurrentStrokes;
        pCurrentStrokes=0;
    }
    updateGL();
}

void Display::mouseReleaseEvent(QMouseEvent *event){
    //cout<<"mouse release"<<endl;
    if(event->button()==Qt::LeftButton) {
        leftMousePressed=false;
        if(pCurrentStrokes)shapeDetection(false);
        delete pExtrudeFrom;
        pExtrudeFrom=0;
        extrudedShape=0;
    }//left mouse button
    else if(event->button()==Qt::RightButton){
        changeCursor();
        rightMousePressed=false;
        delete pDragFrom;
        pDragFrom=0;\
    }
}//mouse release event

void Display::mouseDoubleClickEvent(QMouseEvent *event){
    //mouseReleaseEvent(event);
    if(pCurrentStrokes){
        delete pCurrentStrokes;
        pCurrentStrokes=0;
    }
    //cout<<"mouse double click"<<endl;
}

//selects objects
int Display::selection(){
    int result=-1; //background
    GLuint buffer[BUFFER_SIZE]; //buffer for selection
    GLint hits; //no. of objects selected
    GLint viewport[4];
    glViewport((GLsizei)(width/3.0f+1.0f), 0, (GLsizei)(width/3.0f*2.0f-1),(GLsizei)height);
    glGetIntegerv(GL_VIEWPORT,viewport);
    glSelectBuffer(BUFFER_SIZE,buffer);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(0);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluPickMatrix((GLdouble) mouseX, (GLdouble) (viewport[3]-mouseY), SCANSIZE, SCANSIZE, viewport);
        glOrtho(-1.0f*panelHalfSpanW,panelHalfSpanW,-1.0*panelHalfSpanH,panelHalfSpanH,nearPoint,farPoint);
        glMatrixMode(GL_MODELVIEW);
        drawScene(false);
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    hits=glRenderMode(GL_RENDER);
    if(hits>0){
        result=buffer[3];
        int depth=buffer[1];
        for(int j=1;j<hits;j++){
            if(buffer[j*4+1]<GLuint(depth)){
                result=buffer[j*4+3];
                depth=buffer[j*4+1];
            }
        }//find closest object
    }//objects found
    return result;
}

void Display::drawStrokes(Strokes * strokes){
    glLoadIdentity();
    glTranslatef(drawTranslateX,drawTranslateY,-1.0f);
    glLineWidth(strokeWidth);
    if(strokes){
        Line * tempLine=strokes->getHeadLine();
        do{
            glBegin(GL_LINE_STRIP);
            Point * tempPt=tempLine->getHeadPoint();
                do{
                    glVertex3f(tempPt->getX(),tempPt->getY(),0.0f);
                }while( (tempPt=tempPt->next)!=0);
            glEnd();
        }while((tempLine=tempLine->next)!=0);
    }
}

void Display::drawCone(Cone * cone){
    glRotatef(cone->getXRotation(),1.0,0.0,0.0);
    glRotatef(cone->getYRotation(),0.0,1.0,0.0);
    glTranslatef(cone->getCentre()->getX(),cone->getCentre()->getY(),cone->getCentre()->getZ());
    glRotatef(cone->getZRotation(),0,0,1.0f);
    glRotatef(-90.0f,1,0,0);
    gluCylinder(quadric,cone->getBase()/2.0f,0,cone->getHeight(),sliceInCircle,sliceInCircle);
}

void Display::drawCylinder(Cylinder * cylinder){
    glRotatef(cylinder->getXRotation(),1.0,0.0,0.0);
    glRotatef(cylinder->getYRotation(),0.0,1.0,0.0);
    glTranslatef(cylinder->getCentre()->getX(),cylinder->getCentre()->getY(),cylinder->getCentre()->getZ());
    glRotatef(cylinder->getZRotation(),0,0,1.0f);
    if(cylinder->isTube()){
        glRotatef(90.0f,0,1,0);
        glTranslatef(0,0,cylinder->getLength()/-2.0f);
        if(cylinder->drawBase()){
            glPushMatrix();
            gluDisk(quadric,0,cylinder->getWidth()/2.0f,sliceInCircle,1);
            glTranslatef(0,0,cylinder->getLength());
            gluDisk(quadric,0,cylinder->getWidth()/2.0f,sliceInCircle,1);
            glPopMatrix();
        }
        gluCylinder(quadric,cylinder->getWidth()/2.0f,cylinder->getWidth()/2.0f,cylinder->getLength(),sliceInCircle,sliceInCircle);
    }
    else{
        glRotatef(90.0f,1,0,0);
        glTranslatef(0,0,cylinder->getWidth()/-2.0f);
        if(cylinder->drawBase()){
            glPushMatrix();
            gluDisk(quadric,0,cylinder->getLength()/2.0f,sliceInCircle,1);
            glTranslatef(0,0,cylinder->getWidth());
            gluDisk(quadric,0,cylinder->getLength()/2.0f,sliceInCircle,1);
            glPopMatrix();
        }
        gluCylinder(quadric,cylinder->getLength()/2.0f,cylinder->getLength()/2.0f,cylinder->getWidth(),sliceInCircle,sliceInCircle);
    }
}

void Display::drawRectangle(Rect *rect){
    glRotatef(rect->getXRotation(),1.0,0.0,0.0);
    glRotatef(rect->getYRotation(),0.0,1.0,0.0);
    glTranslatef(rect->getCentre()->getX(),rect->getCentre()->getY(),rect->getCentre()->getZ());
    glRotatef(rect->getZRotation(),0,0,1.0f);
    if(rect->getHeight()==0.0f){
        glBegin(GL_QUADS);
            glVertex3f(-0.5f*rect->getLength(),0.5f*rect->getWidth(),0);
            glVertex3f(-0.5f*rect->getLength(),-0.5f*rect->getWidth(),0);
            glVertex3f(0.5f*rect->getLength(),-0.5f*rect->getWidth(),0);
            glVertex3f(0.5f*rect->getLength(),0.5f*rect->getWidth(),0);
        glEnd();
    }
    else{
        glBegin(GL_QUADS);
            //bottom
            glVertex3f(-0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            glVertex3f(-0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            glVertex3f(0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            glVertex3f(0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            //side1
            glVertex3f(-0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            glVertex3f(-0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            glVertex3f(-0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
            glVertex3f(-0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
            //side2
            glVertex3f(0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            glVertex3f(0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            glVertex3f(0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
            glVertex3f(0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
            //side3
            glVertex3f(-0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            glVertex3f(0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            glVertex3f(0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
            glVertex3f(-0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
            //side4
            glVertex3f(-0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            glVertex3f(0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/2.0f);
            glVertex3f(0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
            glVertex3f(-0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
            //top
            glVertex3f(-0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
            glVertex3f(-0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
            glVertex3f(0.5f*rect->getLength(),-0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
            glVertex3f(0.5f*rect->getLength(),0.5f*rect->getWidth(),rect->getHeight()/-2.0f);
        glEnd();
    }
}

void Display::drawCircle(Circle * circle){
    //glTranslatef(-1.0f*drawTranslateX,-1.0f*drawTranslateY,0.0);
    glRotatef(circle->getXRotation(),1.0,0.0,0.0);
    glRotatef(circle->getYRotation(),0.0,1.0,0.0);
    //glTranslatef(drawTranslateX,drawTranslateY,0.0);
    glTranslatef(circle->getCentre()->getX(),circle->getCentre()->getY(),circle->getCentre()->getZ());
    gluDisk(quadric,0,circle->getRadius(),sliceInCircle,1);
}

void Display::drawSphere(Sphere * sph){
    gluQuadricNormals(quadric,GLU_NONE);
    gluQuadricDrawStyle(quadric,GLU_FILL);
    glRotatef(sph->getXRotation(),1.0,0.0,0.0);
    glRotatef(sph->getYRotation(),0.0,1.0,0.0);
    glTranslatef(sph->getCentre()->getX(),sph->getCentre()->getY(),sph->getCentre()->getZ());
    //glEnable(GL_CULL_FACE);
    gluSphere(quadric,sph->getRadius(),sliceInCircle,sliceInCircle);
    //glDisable(GL_CULL_FACE);
}

void Display::drawTriangle(Triangle * tri){
    glRotatef(tri->getXRotation(),1.0,0.0,0.0);
    glRotatef(tri->getYRotation(),0.0,1.0,0.0);
    if(tri->getHeight()==0.0f){
        glBegin(GL_POLYGON);
            glVertex3f(tri->getP1()->getX(),tri->getP1()->getY(),tri->getP1()->getZ());
            glVertex3f(tri->getP2()->getX(),tri->getP2()->getY(),tri->getP2()->getZ());
            glVertex3f(tri->getP3()->getX(),tri->getP3()->getY(),tri->getP3()->getZ());
        glEnd();
    }
    else{
        glBegin(GL_POLYGON);
            glVertex3f(tri->getP1()->getX(),tri->getP1()->getY(),tri->getP1()->getZ()-tri->getHeight()/2.0f);
            glVertex3f(tri->getP2()->getX(),tri->getP2()->getY(),tri->getP2()->getZ()-tri->getHeight()/2.0f);
            glVertex3f(tri->getP3()->getX(),tri->getP3()->getY(),tri->getP3()->getZ()-tri->getHeight()/2.0f);
        glEnd();
        glBegin(GL_QUADS);
            //side a
            glVertex3f(tri->getP1()->getX(),tri->getP1()->getY(),tri->getP1()->getZ()-tri->getHeight()/2.0f);
            glVertex3f(tri->getP2()->getX(),tri->getP2()->getY(),tri->getP2()->getZ()-tri->getHeight()/2.0f);
            glVertex3f(tri->getP2()->getX(),tri->getP2()->getY(),tri->getP2()->getZ()+tri->getHeight()/2.0f);
            glVertex3f(tri->getP1()->getX(),tri->getP1()->getY(),tri->getP1()->getZ()+tri->getHeight()/2.0f);
            //side b
            glVertex3f(tri->getP2()->getX(),tri->getP2()->getY(),tri->getP2()->getZ()-tri->getHeight()/2.0f);
            glVertex3f(tri->getP3()->getX(),tri->getP3()->getY(),tri->getP3()->getZ()-tri->getHeight()/2.0f);
            glVertex3f(tri->getP3()->getX(),tri->getP3()->getY(),tri->getP3()->getZ()+tri->getHeight()/2.0f);
            glVertex3f(tri->getP2()->getX(),tri->getP2()->getY(),tri->getP2()->getZ()+tri->getHeight()/2.0f);
            //side c
            glVertex3f(tri->getP1()->getX(),tri->getP1()->getY(),tri->getP1()->getZ()-tri->getHeight()/2.0f);
            glVertex3f(tri->getP3()->getX(),tri->getP3()->getY(),tri->getP3()->getZ()-tri->getHeight()/2.0f);
            glVertex3f(tri->getP3()->getX(),tri->getP3()->getY(),tri->getP3()->getZ()+tri->getHeight()/2.0f);
            glVertex3f(tri->getP1()->getX(),tri->getP1()->getY(),tri->getP1()->getZ()+tri->getHeight()/2.0f);
        glEnd();
        glBegin(GL_POLYGON);
            glVertex3f(tri->getP1()->getX(),tri->getP1()->getY(),tri->getP1()->getZ()+tri->getHeight()/2.0f);
            glVertex3f(tri->getP2()->getX(),tri->getP2()->getY(),tri->getP2()->getZ()+tri->getHeight()/2.0f);
            glVertex3f(tri->getP3()->getX(),tri->getP3()->getY(),tri->getP3()->getZ()+tri->getHeight()/2.0f);
        glEnd();
    }
}

void Display::drawFreehand(Freehand *free){
    glRotatef(free->getXRotation(),1.0,0.0,0.0);
    glRotatef(free->getYRotation(),0.0,1.0,0.0);
    Line * line=free->getLine();
    glLineWidth(freehandWidth);
    glBegin(GL_LINE_STRIP);
    Point * tempPt=line->getHeadPoint();
        do{
            glVertex3f(tempPt->getX(),tempPt->getY(),tempPt->getZ());
        }while( (tempPt=tempPt->next)!=0);
    glEnd();
}

void Display::drawDrawPanel(){
    glLoadIdentity();
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glColor4f(1.0f,1.0f,1.0f,0.7f);
    glTranslatef(0,0,panelDistance);
    if(panelMoving){
        glTranslatef(-1.0f*cos(40.0f)*(drawTranslateZ),0,sin(40.0f)*(drawTranslateZ));
        //glTranslatef(0,0,0.01f);
        glRotatef(50.0f,0,1,0);
    }
    else glTranslatef(0,0,drawTranslateZ);
    glBegin(GL_QUADS);
        glVertex3f(-5.0f*panelHalfSpanW, panelHalfSpanH*5.0f,0);	// Top Left
        glVertex3f(panelHalfSpanW*5.0f, panelHalfSpanH*5.0f, 0);	// Top Right
        glVertex3f(panelHalfSpanW*5.0f,-5.0f*panelHalfSpanH,0);// Bottom Right
        glVertex3f(-5.0f*panelHalfSpanW,-5.0f*panelHalfSpanH,0);	// Bottom Left
    glEnd();
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void Display::drawText(){
    glViewport(0,0,(GLsizei)width,(GLsizei)height);
    glColor3f(0.0,0.0,0.6);
    renderText(0,12,"<Viewer>",QFont( "Helvetica", 10, QFont::Bold,false),0);
    ostringstream stream1;
    stream1<<"Panel depth:"<<drawTranslateZ;
    renderText((GLsizei)(width/3.0f+1),12,"<Drawing Panel> Move cursor to Viewer or right-click for quick detection",QFont( "Helvetica", 10, QFont::Bold,false),0);
    renderText((GLsizei)(width-110.0f),(GLsizei)(height-8),stream1.str().c_str(),QFont( "Helvetica", 10, QFont::Bold,false),0);
    if(pCurrentStrokes){
        ostringstream stream;
        stream<<pCurrentStrokes->getTailLine()->getTotalPoints()<<" points in line";
        renderText((GLsizei)(width/3.0f+1),24,stream.str().c_str(),QFont( "Helvetica", 10, QFont::Bold,false),0);
    }
    //Red warning text
    glColor3f(0.9,0.0,0.0);
    renderText((GLsizei)(width/3.0f+1),(GLsizei)height-8,statusString.c_str(),QFont( "Helvetica", 10, QFont::Bold,false),0);
}//drawText

GLfloat Display::windowToSceneH(int input){
      return (GLfloat)((GLfloat)input-height/2.0)/(height/2.0)*(panelHalfSpanH/-1.0f)-drawTranslateY;
}

GLfloat Display::windowToSceneW(int input){
     return (GLfloat)(((GLfloat)input-width/3.0f-1.0f)-(width/3.0f))/(width/3.0f)*(panelHalfSpanW)-drawTranslateX;
}

void Display::drawMidLine(){
    glViewport((GLsizei)(width/3.0f-1), 0, 2, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);	// Select The Projection Matrix
        glLoadIdentity();// Reset The Projection Matrix
    glOrtho(-1.0f,1.0f,-1.0f,1.0f,nearPoint,farPoint);

    glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
        glLoadIdentity();
    glClear (GL_DEPTH_BUFFER_BIT);
    glColor3f(0.5,0.5,0.5);
    glBegin(GL_QUADS);						// Draw A Quad
        glVertex3f(-10.0f, 10.0f, -2.0f);				// Top Left
        glVertex3f( 10.0f, 10.0f, -2.0f);				// Top Right
        glVertex3f( 10.0f,-10.0f, -2.0f);				// Bottom Right
        glVertex3f(-10.0f,-10.0f, -2.0f);				// Bottom Left
    glEnd();
}

void Display::clearScene(){
    //confirm Dialog Yes/No
    //save scene?
    pScene->clear();
    if(pCurrentStrokes){
        delete pCurrentStrokes;
        pCurrentStrokes=0;
    }
    if(pLastStrokes){
        delete pLastStrokes;
        pLastStrokes=0;
    }
    initializeValues();
    updateGL();
}

void Display::resizeGL(int width, int height){
    if (height==0) height=1;//prevents zero division
    if(width==0) width=1;
    this->width=width;
    this->height=height;
}

void Display::modeChanged(int mode){
    if(pLastStrokes) delete pLastStrokes;
    pLastStrokes=pCurrentStrokes;
    pCurrentStrokes=0;
    this->mode=mode;
    changeCursor();
}

bool Display::isDrawMode(){
    if(mode<MainWindow::mode_push_pull) return true;
    else return false;
}

void Display::initializeGL(){
    glClearColor( 1.0, 1.0, 1.0, 0.0 );
    glEnable(GL_LINE_SMOOTH);
    glEnable (GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);
    quadric=gluNewQuadric();
}

void Display::xRotation(int deg){
    if(deg!=0){
        drawRotateX=originalRotateX+deg;
        //rotatingX=true;
        updateGL();
    }
    else originalRotateX=drawRotateX;
}

void Display::yRotation(int deg){
    if(deg!=0){
        drawRotateY=originalRotateY+deg;
        //rotatingX=false;
        updateGL();
    }
    else originalRotateY=drawRotateY;
}

void Display::zoomIn(){
    statusString="";
    if(drawPanelZoom>zoomInterval+0.1f) drawPanelZoom-=zoomInterval;
    else statusString="Unable to zoom in anymore!";
    updateGL();
}

void Display::zoomOut(){
    statusString="";
    drawPanelZoom+=zoomInterval;
    updateGL();
}

bool Display::isInDrawPanel(){
    if(mouseX>width*1.0f/3.0f)
        return true;
    else return false;
}

void Display::panelIn(){
    //statusString="";
    panelMoving=true;
    drawTranslateZ-=panelMoveInterval;
    updateGL();
}

void Display::panelOut(){
    //statusString="";
    panelMoving=true;
    if(drawTranslateZ+panelMoveInterval<abs(panelDistance)-2.0f)
        drawTranslateZ+=panelMoveInterval;
    else statusString="Draw panel too close to camera!";
    updateGL();
}

void Display::panelMoveStopped(){
    panelMoving=false;
    //updateGL();
}

void Display::undo(){
    if(pCurrentStrokes){
        delete pLastStrokes;
        pLastStrokes=pCurrentStrokes;
        pCurrentStrokes=0;
    }
    else pScene->removeLast();
    updateGL();
}

void Display::changeCursor(){
    if(isDrawMode()){
        setCursor(Qt::CrossCursor);
    }
    else if(mode==MainWindow::mode_delete){
        setCursor(Qt::ArrowCursor);
    }
}
