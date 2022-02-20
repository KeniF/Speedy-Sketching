#ifndef SCENE_H
#define SCENE_H
#include <Vector>
#include <object.h>
#include <shape.h>
using namespace std;
class Scene{
public:
    Scene();
    void addObject(Object * obj);
    void removeObject(int index);
    void removeLast();
    void clear();
    Shape* getShape(int index);
    vector<Object*> getAll();
private:
    vector<Object*> v;
};

#endif // SCENE_H
