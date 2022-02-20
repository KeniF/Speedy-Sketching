#ifndef CIRCLE_H
#define CIRCLE_H
#include "shape.h"
#include "point.h"

class Circle:public Shape
{
public:
    Circle(Point * centre, GLfloat radius,GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);
    virtual ~Circle();
    Point * getCentre() const {return centre;}
    GLfloat getRadius() const{return radius;}
    void setRadius(GLfloat in){radius=in;}
    void setCentre(Point * in){centre=in;}
    virtual string toString();
private:
    Point * centre;
    GLfloat radius;
};

#endif // CIRCLE_H
