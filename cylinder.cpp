#include "cylinder.h"
#include "OpenGL/glu.h"

Cylinder::Cylinder(bool isTube, bool hasBase, Point *centre, GLfloat width,
                   GLfloat length, GLfloat xRotation, GLfloat yRotation,
                   GLfloat zRotation)
    : centre(centre), length(length), width(width), tube(isTube),
      hasBase(hasBase), Shape(xRotation, yRotation, zRotation) {}

Cylinder::~Cylinder() { delete centre; }

void Cylinder::draw(GLUquadricObj *quadric, bool selected) {
  if (selected)
    glColor4f(1.0f, 0.7, 0.1, 1.0);
  else
    glColor4f(1.0f, 0.5, 0.1, 1.0);

  glRotatef(xRotation, 1.0, 0.0, 0.0);
  glRotatef(yRotation, 0.0, 1.0, 0.0);
  glTranslatef(centre->getX(), centre->getY(), centre->getZ());
  glRotatef(zRotation, 0, 0, 1.0f);
  if (tube) {
    glRotatef(90.0f, 0, 1, 0);
    drawCylinder(quadric, width, length);
  } else {
    glRotatef(90.0f, 1, 0, 0);
    drawCylinder(quadric, length, width);
  }
}

void Cylinder::drawCylinder(GLUquadricObj *quadric, float radius, float height) {
  glTranslatef(0, 0, height / -2.0f);
  if (hasBase) {
    glPushMatrix();
    gluDisk(quadric, 0, radius / 2.0f, sliceInCircle, 1);
    glTranslatef(0, 0, height);
    gluDisk(quadric, 0, radius / 2.0f, sliceInCircle, 1);
    glPopMatrix();
  }
  gluCylinder(quadric, radius / 2.0f, radius / 2.0f, height, sliceInCircle,
              sliceInCircle);
}
