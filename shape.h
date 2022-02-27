#ifndef SHAPE_H
#define SHAPE_H
#include "OpenGL/gl.h"
#include <QOpenGLWidget>
#include "OpenGL/glu.h"

class Shape
{
public:
    Shape(GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);
    virtual ~Shape();
    void setHeight(GLfloat in){ height=in; }
    GLfloat getHeight(){ return height;}
    GLfloat getXRotation(){return xRotation;}
    GLfloat getYRotation(){return yRotation;}
    GLfloat getZRotation(){return zRotation;}
    virtual void draw(GLUquadricObj * quadric, bool selected) = 0;
protected:
    GLfloat height, xRotation, yRotation, zRotation;
};

#endif // SHAPE_H
