#ifndef LINE_H
#define LINE_H
#include <iostream>
#include <point.h>
#include <shape.h>
#include <sstream>
#include <string>
class Line : public Shape {
public:
  Line(Point *head, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);
  Line(const Line &);
  virtual ~Line();
  void addPoint(Point *point);
  Point *getHeadPoint() { return pHead; }
  Point *getTail() { return pTail; }
  virtual string toString();
  int getTotalPoints() { return totalPoints; }
  GLfloat getMinX() { return minX; }
  GLfloat getMaxX() { return maxX; }
  GLfloat getMinY() { return minY; }
  GLfloat getMaxY() { return maxY; }
  Line* getNext() { return next; }
  void setNext(Line* line) { next = line; }
  void draw(GLUquadricObj *quadric, bool selected);

private:
  Point *pHead, *pTail;
  Line *next;
  int totalPoints;
  GLfloat minX, maxX, minY, maxY;
};

#endif // LINE_H
