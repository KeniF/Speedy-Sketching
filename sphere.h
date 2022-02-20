#ifndef SPHERE_H
#define SPHERE_H
#include "shape.h"
#include "point.h"

using namespace std;
class Sphere:public Shape
{
public:
    Sphere(Point * centre, GLfloat radius, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);
    virtual ~Sphere();
    GLfloat getRadius(){return radius;}
    Point * getCentre(){return centre;}
private:
    GLfloat radius;
    Point * centre;
};

#endif // SPHERE_H
