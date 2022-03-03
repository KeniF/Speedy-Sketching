#include "triangle.h"

Triangle::Triangle(Point *p1, Point *p2, Point *p3, GLfloat xRotation,
                   GLfloat yRotation, GLfloat zRotation)
    : pPoint1(p1), pPoint2(p2), pPoint3(p3),
      Shape(xRotation, yRotation, zRotation) {}

Triangle::~Triangle() {
  delete pPoint1;
  delete pPoint2;
  delete pPoint3;
}

void Triangle::draw(GLUquadricObj *quadric, bool selected) {
  if (selected)
    glColor4f(0.3, 0.9, 0.3, 1.0);
  else
    glColor4f(0, 0.7, 0, 1.0);

  glRotatef(xRotation, 1.0, 0.0, 0.0);
  glRotatef(yRotation, 0.0, 1.0, 0.0);
  if (height == 0.0f) {
    glBegin(GL_POLYGON);
    glVertex3f(pPoint1->getX(), pPoint1->getY(), pPoint1->getZ());
    glVertex3f(pPoint2->getX(), pPoint2->getY(), pPoint2->getZ());
    glVertex3f(pPoint2->getX(), pPoint3->getY(), pPoint3->getZ());
    glEnd();
  } else {
    glBegin(GL_POLYGON);
    glVertex3f(pPoint1->getX(), pPoint1->getY(),
               pPoint1->getZ() - height / 2.0f);
    glVertex3f(pPoint2->getX(), pPoint2->getY(),
               pPoint2->getZ() - height / 2.0f);
    glVertex3f(pPoint3->getX(), pPoint3->getY(),
               pPoint3->getZ() - height / 2.0f);
    glEnd();
    glBegin(GL_QUADS);
    // side a
    glVertex3f(pPoint1->getX(), pPoint1->getY(),
               pPoint1->getZ() - height / 2.0f);
    glVertex3f(pPoint2->getX(), pPoint2->getY(),
               pPoint2->getZ() - height / 2.0f);
    glVertex3f(pPoint2->getX(), pPoint2->getY(),
               pPoint2->getZ() + height / 2.0f);
    glVertex3f(pPoint1->getX(), pPoint1->getY(),
               pPoint1->getZ() + height / 2.0f);
    // side b
    glVertex3f(pPoint2->getX(), pPoint2->getY(),
               pPoint2->getZ() - height / 2.0f);
    glVertex3f(pPoint3->getX(), pPoint3->getY(),
               pPoint3->getZ() - height / 2.0f);
    glVertex3f(pPoint3->getX(), pPoint3->getY(),
               pPoint3->getZ() + height / 2.0f);
    glVertex3f(pPoint2->getX(), pPoint2->getY(),
               pPoint2->getZ() + height / 2.0f);
    // side c
    glVertex3f(pPoint1->getX(), pPoint1->getY(),
               pPoint1->getZ() - height / 2.0f);
    glVertex3f(pPoint3->getX(), pPoint3->getY(),
               pPoint3->getZ() - height / 2.0f);
    glVertex3f(pPoint3->getX(), pPoint3->getY(),
               pPoint3->getZ() + height / 2.0f);
    glVertex3f(pPoint1->getX(), pPoint1->getY(),
               pPoint1->getZ() + height / 2.0f);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex3f(pPoint1->getX(), pPoint1->getY(),
               pPoint1->getZ() + height / 2.0f);
    glVertex3f(pPoint2->getX(), pPoint2->getY(),
               pPoint2->getZ() + height / 2.0f);
    glVertex3f(pPoint3->getX(), pPoint3->getY(),
               pPoint3->getZ() + height / 2.0f);
    glEnd();
  }
}
