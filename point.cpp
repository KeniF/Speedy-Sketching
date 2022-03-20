#include "point.h"

Point::Point(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z), next(0) {}

Point::Point(const Point &rhs):
  x(rhs.x), y(rhs.y), z(rhs.z), next(rhs.next), last(rhs.last)  {
}

Point::~Point() {
  delete next;
  next = 0;
  last = 0;
}

string Point::toString() {
  ostringstream stream;
  stream << "<Point>" << x << "," << y << "," << z;
  return stream.str();
}
