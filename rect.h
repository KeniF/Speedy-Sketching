#ifndef RECT_H
#define RECT_H
#include "OpenGL/glu.h"
#include "point.h"
#include "shape.h"

using namespace std;
class Rect : public Shape {
public:
  Rect(Point *centre, GLfloat width, GLfloat length, GLfloat xRotation,
       GLfloat yRotation, GLfloat zRotation);
  virtual ~Rect();
  Point *getCentre() { return centre; }
  GLfloat getWidth() { return width; }
  GLfloat getLength() { return length; }
  void draw(GLUquadricObj *quadric, bool selected);

private:
  Point *centre;
  GLfloat width, length;
};

#endif // RECTANGLE_H
