#ifndef CYLINDER_H
#define CYLINDER_H
#include "OpenGL/glu.h"
#include "point.h"
#include "shape.h"

class Cylinder : public Shape {
public:
  Cylinder(bool isTube, bool hasBase, Point *centre, GLfloat width,
           GLfloat length, GLfloat xRotation, GLfloat yRotation,
           GLfloat zRotation);
  virtual ~Cylinder();
  Point *getCentre() { return centre; }
  GLfloat getWidth() { return width; }
  GLfloat getLength() { return length; }
  bool isTube() { return tube; }
  bool drawBase() { return hasBase; }
  void draw(GLUquadricObj *quadric, bool selected);

private:
  Point *centre;
  GLfloat width, length;
  bool tube, hasBase;
  static const GLuint sliceInCircle = 200;
};

#endif // CYLINDER_H
