#include "scene.h"
#include <iostream>
using namespace std;
Scene::Scene() {}

void Scene::addObject(Shape *obj) {
  if (obj)
    v.push_back(obj);
}

void Scene::removeObject(int index) {
  delete *(v.begin() + index);
  v.erase(v.begin() + index);
}

void Scene::removeLast() {
  if (v.size()) {
    delete (v.back());
    v.pop_back();
  }
}

Shape *Scene::getShape(int index) {
  return static_cast<Shape *>(*(v.begin() + index));
}

void Scene::clear() {
  for (unsigned int i = 0; i < v.size(); i++)
    delete v[i];
  v.clear();
}

void Scene::drawAll(GLUquadricObj *quadric, GLint selectedObject) {
  for (int i = v.size() - 1; i >= 0; i--) {
    glPushMatrix();
    Shape *temp = v[i];
    glLoadName(i); // needed for picking
    temp->draw(quadric, i == selectedObject);
    glPopMatrix();
  }}
