#ifndef CIRCLE_H
#define CIRCLE_H
#include "OpenGL/glu.h"
#include "point.h"
#include "shape.h"

class Circle : public Shape {
public:
  Circle(Point *centre, GLfloat radius, GLfloat xRotation, GLfloat yRotation,
         GLfloat zRotation);
  virtual ~Circle();
  Point *getCentre() const { return centre; }
  GLfloat getRadius() const { return radius; }
  void setRadius(GLfloat in) { radius = in; }
  void setCentre(Point *in) { centre = in; }
  virtual string toString();
  void draw(GLUquadricObj *quadric, bool selected);

protected:
  Point *centre;
  GLfloat radius;

private:
  static const GLuint noOfSlices = 200;
};

#endif // CIRCLE_H
