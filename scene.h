#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include <shape.h>
using namespace std;
class Scene{
public:
    Scene();
    void addObject(Shape * obj);
    void removeObject(int index);
    void removeLast();
    void clear();
    Shape* getShape(int index);
    vector<Shape*> getAll();
private:
    vector<Shape*> v;
};

#endif // SCENE_H
