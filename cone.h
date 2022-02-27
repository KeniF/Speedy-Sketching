#ifndef CONE_H
#define CONE_H
#include "shape.h"
#include "point.h"
#include "OpenGL/glu.h"

class Cone: public Shape
{
public:
    Cone(Point * centre, GLfloat base, GLfloat height, GLfloat xRotation, GLfloat yRotation, GLfloat  zRotation);
    virtual ~Cone();
    GLfloat getBase(){return base;}
    GLfloat getHeight(){return height;}
    Point * getCentre(){return centre;}
    void draw(GLUquadricObj *quadric, bool selected);
private:
    Point * centre;
    GLfloat base,height;
    static const GLuint sliceInCircle = 200;
};

#endif // CONE_H
