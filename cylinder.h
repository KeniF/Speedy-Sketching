#ifndef CYLINDER_H
#define CYLINDER_H
#include "shape.h"
#include "point.h"

class Cylinder: public Shape
{
public:
    Cylinder(bool isTube,bool hasBase,Point * centre,GLfloat width, GLfloat length,GLfloat xRotation, GLfloat yRotation, GLfloat  zRotation);
    virtual ~Cylinder();
    Point * getCentre(){return centre;}
    GLfloat getWidth(){return width;}
    GLfloat getLength(){return length;}
    bool isTube(){return tube;}
    bool drawBase(){return hasBase;}
private:
    Point * centre;
    GLfloat width, length;
    bool tube, hasBase;
};

#endif // CYLINDER_H
