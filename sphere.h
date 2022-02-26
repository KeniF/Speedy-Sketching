#ifndef SPHERE_H
#define SPHERE_H
#include "shape.h"
#include "point.h"
#include "OpenGL/glu.h"

using namespace std;
class Sphere:public Shape
{
public:
    Sphere(Point * centre, GLfloat radius, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);
    virtual ~Sphere();
    GLfloat getRadius(){return radius;}
    Point * getCentre(){return centre;}
    void draw(GLUquadricObj *quadric, bool selected);
private:
    GLfloat radius;
    Point * centre;
    static const GLuint sliceInSphere = 200;
};

#endif // SPHERE_H
