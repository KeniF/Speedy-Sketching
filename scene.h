#ifndef SCENE_H
#define SCENE_H
#include <shape.h>
#include <vector>
#include "OpenGL/glu.h"
#include "OpenGL/gl.h"

using namespace std;
class Scene {
public:
  Scene();
  void addObject(Shape *obj);
  void removeObject(int index);
  void removeLast();
  void clear();
  Shape *getShape(int index);
  void drawAll(GLUquadricObj *quadric, GLint selectedObject);

private:
  vector<Shape *> v;
};

#endif // SCENE_H
