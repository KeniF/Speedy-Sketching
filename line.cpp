#include "line.h"
#include <iostream>

using namespace std;
Line::Line(Point *head, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation)
    : Shape(xRotation, yRotation, zRotation) {
  pTail = head;
  pHead = head;
  next = 0;
  totalPoints = 1;
  minX = head->getX();
  maxX = head->getX();
  minY = head->getY();
  maxY = head->getY();
}

Line::Line(const Line &rhs)
    : Shape(rhs.xRotation, rhs.yRotation, rhs.zRotation) {
  pHead = new Point(*(rhs.pHead));
  Point *from = rhs.pHead;
  Point *to = pHead;
  Point *tail = pHead;
  totalPoints = rhs.totalPoints;
  while ((from = from->getNext())) {
    to = new Point(*from);
    tail->setNext(to);
    to->setLast(tail);
    tail = to;
  }
  pTail = rhs.pTail;
  minX = rhs.minX;
  maxX = rhs.maxX;
  minY = rhs.minY;
  maxY = rhs.maxY;
}

Line::~Line() {
  delete pHead;
  delete next;
}

void Line::addPoint(Point *point) {
  pTail->setNext(point);
  if (point->getX() < minX)
    minX = point->getX();
  if (point->getX() > maxX)
    maxX = point->getX();
  if (point->getY() < minY)
    minY = point->getY();
  if (point->getY() > maxY)
    maxY = point->getY();
  pTail = point;
  totalPoints++;
} // addPoint

string Line::toString() {
  ostringstream stream;
  stream << "<Line>" << getTotalPoints() << "\n";
  Point *iterator = this->getHeadPoint();
  do {
    stream << iterator->toString();
  } while ((iterator = iterator->getNext()));
  return stream.str();
}

void Line::draw(GLUquadricObj *quadric, bool selected) {}
