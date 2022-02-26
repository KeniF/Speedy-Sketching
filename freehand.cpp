#include "freehand.h"

Freehand::Freehand(bool closed, Line *line, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation):
        Shape(xRotation,yRotation,zRotation){
    this->closed=closed;
    this->line=line;
}

Freehand::~Freehand(){
    delete line;
    line=0;
}

void Freehand::draw(GLUquadricObj *quadric, bool selected) {
    if (selected) glColor4f(0.1,0.1,0.8,1.0);
    else glColor4f(0,0.0,0.4,1.0);

    glRotatef(xRotation, 1.0, 0.0, 0.0);
    glRotatef(yRotation, 0.0, 1.0, 0.0);
    glLineWidth(freehandWidth);
    glBegin(GL_LINE_STRIP);
    Point * tempPt=line->getHeadPoint();
        do{
            glVertex3f(tempPt->getX(),tempPt->getY(),tempPt->getZ());
        }while( (tempPt=tempPt->next)!=0);
    glEnd();
}
