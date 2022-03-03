#include "rect.h"

Rect::Rect(Point *centre, GLfloat width, GLfloat length, GLfloat xRotation,
           GLfloat yRotation, GLfloat zRotation)
    : centre(centre), length(length), width(width),
      Shape(xRotation, yRotation, zRotation) {}

Rect::~Rect() { delete centre; }

void Rect::draw(GLUquadricObj *quadric, bool selected) {
  if (selected)
    glColor4f(0.95, 0.95, 0.2, 1.0);
  else
    glColor4f(0.85, 0.85, 0.2, 1.0);

  glRotatef(xRotation, 1.0, 0.0, 0.0);
  glRotatef(yRotation, 0.0, 1.0, 0.0);
  glTranslatef(centre->getX(), centre->getY(), centre->getZ());
  glRotatef(zRotation, 0, 0, 1.0f);
  if (height == 0.0f) {
    glBegin(GL_QUADS);
    glVertex3f(-0.5f * length, 0.5f * width, 0);
    glVertex3f(-0.5f * length, -0.5f * width, 0);
    glVertex3f(0.5f * length, -0.5f * width, 0);
    glVertex3f(0.5f * length, 0.5f * width, 0);
    glEnd();
  } else {
    glBegin(GL_QUADS);
    // bottom
    glVertex3f(-0.5f * length, 0.5f * width, height / 2.0f);
    glVertex3f(-0.5f * length, -0.5f * width, height / 2.0f);
    glVertex3f(0.5f * length, -0.5f * width, height / 2.0f);
    glVertex3f(0.5f * length, 0.5f * width, height / 2.0f);
    // side1
    glVertex3f(-0.5f * length, 0.5f * width, height / 2.0f);
    glVertex3f(-0.5f * length, -0.5f * width, height / 2.0f);
    glVertex3f(-0.5f * length, -0.5f * width, height / -2.0f);
    glVertex3f(-0.5f * length, 0.5f * width, height / -2.0f);
    // side2
    glVertex3f(0.5f * length, -0.5f * width, height / 2.0f);
    glVertex3f(0.5f * length, 0.5f * width, height / 2.0f);
    glVertex3f(0.5f * length, 0.5f * width, height / -2.0f);
    glVertex3f(0.5f * length, -0.5f * width, height / -2.0f);
    // side3
    glVertex3f(-0.5f * length, -0.5f * width, height / 2.0f);
    glVertex3f(0.5f * length, -0.5f * width, height / 2.0f);
    glVertex3f(0.5f * length, -0.5f * width, height / -2.0f);
    glVertex3f(-0.5f * length, -0.5f * width, height / -2.0f);
    // side4
    glVertex3f(-0.5f * length, 0.5f * width, height / 2.0f);
    glVertex3f(0.5f * length, 0.5f * width, height / 2.0f);
    glVertex3f(0.5f * length, 0.5f * width, height / -2.0f);
    glVertex3f(-0.5f * length, 0.5f * width, height / -2.0f);
    // top
    glVertex3f(-0.5f * length, 0.5f * width, height / -2.0f);
    glVertex3f(-0.5f * length, -0.5f * width, height / -2.0f);
    glVertex3f(0.5f * length, -0.5f * width, height / -2.0f);
    glVertex3f(0.5f * length, 0.5f * width, height / -2.0f);
    glEnd();
  }
}
