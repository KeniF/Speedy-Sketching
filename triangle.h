#ifndef TRIANGLE_H
#define TRIANGLE_H
#include "point.h"
#include "shape.h"
#include "OpenGL/glu.h"

class Triangle: public Shape{
public:
    Triangle(Point * p1, Point * p2, Point * p3,GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);
    virtual ~Triangle();
    Point * getP1(){return pPoint1;}
    Point * getP2(){return pPoint2;}
    Point * getP3(){return pPoint3;}
    void draw(GLUquadricObj *quadric, bool selected);
private:
    Point * pPoint1, * pPoint2, * pPoint3;
};

#endif // TRIANGLE_H
