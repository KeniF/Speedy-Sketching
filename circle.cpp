#include "circle.h"
#include "iostream"
#include "shape.h"

using namespace std;
Circle::Circle(Point *centre, GLfloat radius, GLfloat xRotation,
               GLfloat yRotation, GLfloat zRotation)
    : centre(centre), radius(radius), Shape(xRotation, yRotation, zRotation) {}

Circle::~Circle() { delete centre; }

string Circle::toString() {
  ostringstream stream;
  stream << "<Circle> Radius:" << radius << "Centre: " << centre->toString()
         << endl;
  string output = stream.str();
  return output;
}

void Circle::draw(GLUquadricObj *quadric, bool selected) {
  if (selected)
    glColor4f(1.0, 0.4, 0.4, 1.0);
  else
    glColor4f(0.9, 0.0, 0.0, 1.0);
  glRotatef(xRotation, 1.0, 0.0, 0.0);
  glRotatef(yRotation, 0.0, 1.0, 0.0);
  glTranslatef(centre->getX(), centre->getY(), centre->getZ());
  gluDisk(quadric, 0, radius, noOfSlices, 1);
}
