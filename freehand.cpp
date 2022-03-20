#include "freehand.h"

Freehand::Freehand(bool closed, Line *line, GLfloat xRotation,
                   GLfloat yRotation, GLfloat zRotation)
    : closed(closed), line(line), Shape(xRotation, yRotation, zRotation) {}

Freehand::~Freehand() { delete line; }

void Freehand::draw(GLUquadricObj *quadric, bool selected) {
  if (selected)
    glColor4f(0.1, 0.1, 0.8, 1.0);
  else
    glColor4f(0, 0.0, 0.4, 1.0);

  glRotatef(xRotation, 1.0, 0.0, 0.0);
  glRotatef(yRotation, 0.0, 1.0, 0.0);
  glLineWidth(freehandWidth);
  glBegin(GL_LINE_STRIP);
  Point *tempPt = line->getHeadPoint();
  do {
    glVertex3f(tempPt->getX(), tempPt->getY(), tempPt->getZ());
  } while ((tempPt = tempPt->getNext()) != 0);
  glEnd();
}
