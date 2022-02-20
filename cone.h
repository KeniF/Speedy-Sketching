#ifndef CONE_H
#define CONE_H
#include "shape.h"
#include "point.h"

class Cone: public Shape
{
public:
    Cone(Point * centre, GLfloat base, GLfloat height, GLfloat xRotation, GLfloat yRotation, GLfloat  zRotation);
    virtual ~Cone();
    GLfloat getBase(){return base;}
    GLfloat getHeight(){return height;}
    Point * getCentre(){return centre;}
private:
    Point * centre;
    GLfloat base,height;
};

#endif // CONE_H
