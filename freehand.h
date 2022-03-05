#ifndef FREEHAND_H
#define FREEHAND_H
#include "OpenGL/glu.h"
#include "line.h"
#include "shape.h"

class Freehand : public Shape {
public:
  Freehand(bool closed, Line *line, GLfloat xRotation, GLfloat yRotation,
           GLfloat zRotation);
  ~Freehand();
  bool isClosedShape() { return closed; }
  Line *getLine() { return line; }
  void draw(GLUquadricObj *quadric, bool selected);

private:
  bool closed;
  Line *line;
  constexpr static const GLfloat freehandWidth = 2.0f;
};

#endif // FREEHAND_H
