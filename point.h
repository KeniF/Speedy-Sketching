#ifndef POINT_H
#define POINT_H
#include <QOpenGLWidget>
#include <sstream>
#include <string>

using namespace std;
class Point {
public:
  Point(GLfloat x, GLfloat y, GLfloat z);
  // Point();
  Point(const Point &);
  ~Point();
  void setNext(Point *next) { this->next = next; }
  Point *getNext(Point *next) { return this->next; }
  Point *getLast(Point *last) { return this->last; }
  void setLast(Point *last) { this->last = last; }
  GLfloat getX() const { return x; }
  GLfloat getY() const { return y; }
  GLfloat getZ() const { return z; }
  Point *next, *last;
  string toString();

private:
  GLfloat x, y, z;
};

#endif // POINT_H
