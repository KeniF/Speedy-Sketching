#ifndef DISPLAY_H
#define DISPLAY_H
#include "OpenGL/glu.h"
#include "circle.h"
#include "cone.h"
#include "cylinder.h"
#include "freehand.h"
#include "line.h"
#include "rect.h"
#include "scene.h"
#include "sphere.h"
#include "strokes.h"
#include "triangle.h"
#include <QMainWindow>
#include <QOpenGLWidget>
#include <sstream>
#include <string>
/* Display is an implementation of OpenGL*/

class Display : public QOpenGLWidget {
  Q_OBJECT
public:
  Display(QMainWindow *parent);
  ~Display();

  enum class Mode { freehand = 1, circle = 6, triangle = 11,
                    rectangle = 16, cone = 21, cube = 26,
                    cylinder_tube = 31, cylinder_drum = 36,
                    sphere = 41, push_pull = 46, deleting = 51 };
  void zoomIn();
  void zoomOut();
  void panelIn();
  void panelOut();
  void clearScene();
  void xRotation(int deg);
  void yRotation(int deg);
  void modeChanged(Mode mode);
  void panelMoveStopped();
  void undo();
  void enableDrawPanel(bool in) {
    drawPanelEnabled = in;
    update();
  }
  void initializeValues();
  void enableBase(bool in) { drawBase = in; }


protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  Point *pDragFrom, *pExtrudeFrom;
  Strokes *pCurrentStrokes, *pLastStrokes; // current/last line user is
                                           // sketching
  GLint lastPixelX, lastPixelY;
  bool leftMousePressed, rightMousePressed, panelMoving, drawPanelEnabled,
      drawBase, rotatingX;
  GLfloat panelHalfSpanH, panelHalfSpanW,
      drawPanelZoom, // bigger number=zoom out
      width, height, drawTranslateX, drawTranslateY,
      drawTranslateZ; // drawTranslateZ=panel depth
  Display::Mode mode;
  GLint drawRotateX, drawRotateY, originalRotateX, originalRotateY,
      selectedObject;
  GLuint mouseX, mouseY;
  Scene *pScene;
  string statusString;
  GLUquadricObj *quadric;
  // Vec3f vec, origin;
  Shape *extrudedShape; // store shape being extruded
  QMainWindow *parent;

  void drawDrawPanel();
  void drawScene(bool drawPanel);
  void drawMidLine();
  void drawBoundRect();
  void drawText();
  void drawStrokes(Strokes *strokes);
  void moveCurrentStrokesToLast();
  bool isDrawMode();
  bool isInDrawPanel();
  int selection();
  void changeCursor();
  GLfloat windowToSceneW(int i);
  GLfloat windowToSceneH(int i);
  void drawCone(Cone *cone);
  void drawRectangle(Rect *rect);
  void drawSphere(Sphere *sphere);
  void drawCylinder(Cylinder *cylinder);
  void drawCircle(Circle *circle); // OpenGL does not support drawing of circles
  void drawTriangle(Triangle *tri);
  void drawFreehand(Freehand *free);
  void shapeDetection(bool userTriggered);
};
#endif // DISPLAY_H
