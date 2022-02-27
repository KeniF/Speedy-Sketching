#ifndef DISPLAY_H
#define DISPLAY_H
#include <QOpenGLWidget>
#include <QMainWindow>
#include "line.h"
#include "strokes.h"
#include "scene.h"
#include "circle.h"
#include "triangle.h"
#include "freehand.h"
#include "sphere.h"
#include "rect.h"
#include "cylinder.h"
#include "cone.h"
#include <string>
#include <sstream>
#include "OpenGL/glu.h"
/* Display is an implementation of OpenGL*/

class Display : public QOpenGLWidget
{
    Q_OBJECT
public:
    Display(QMainWindow *parent, int mode);
    ~Display();
    void zoomIn();
    void zoomOut();
    void panelIn();
    void panelOut();
    void clearScene();
    void xRotation(int deg);
    void yRotation(int deg);
    void modeChanged(int mode);
    void panelMoveStopped();
    void undo();
    void enableDrawPanel(bool in){drawPanelEnabled=in; update(); }
    void initializeValues();
    void enableBase(bool in){drawBase=in;}

protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    Point * pDragFrom, * pExtrudeFrom;
    Strokes * pCurrentStrokes, * pLastStrokes; //current/last line user is sketching
    GLint lastPixelX,lastPixelY;
    bool leftMousePressed, rightMousePressed, panelMoving, drawPanelEnabled, drawBase, rotatingX;
    GLfloat panelHalfSpanH, panelHalfSpanW, drawPanelZoom,//bigger number=zoom out
        width, height, drawTranslateX, drawTranslateY,drawTranslateZ;//drawTranslateZ=panel depth
    GLint mode, drawRotateX,drawRotateY, originalRotateX, originalRotateY, selectedObject;
    GLuint mouseX, mouseY;
    Scene * pScene;
    string statusString;
    GLUquadricObj *quadric;
    //Vec3f vec, origin;
    Shape* extrudedShape; //store shape being extruded
    QMainWindow* parent;

    void drawDrawPanel();
    void drawScene(bool drawPanel);
    void drawMidLine();
    void drawBoundRect();
    void drawText();
    void drawStrokes(Strokes * strokes);
    bool isDrawMode();
    bool isInDrawPanel();
    int selection();
    void changeCursor();
    GLfloat windowToSceneW(int i);
    GLfloat windowToSceneH(int i);
    void drawCone(Cone *cone);
    void drawRectangle(Rect *rect);
    void drawSphere(Sphere * sphere);
    void drawCylinder(Cylinder * cylinder);
    void drawCircle(Circle * circle); //OpenGL does not support drawing of circles
    void drawTriangle(Triangle * tri);
    void drawFreehand(Freehand *free);
    void shapeDetection(bool userTriggered);
};
#endif // DISPLAY_H
