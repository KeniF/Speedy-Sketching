#ifndef SHAPE_H
#define SHAPE_H
#include "object.h"
#include "OpenGL/gl.h"
class Shape:public Object
{
public:
    Shape(GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);
    Shape();
    virtual ~Shape();
    //virtual string * toString();
    //void setPosHeight(GLfloat in){ posHeight=in;}
    //void setNegHeight(GLfloat in){ negHeight=in;}
    //GLfloat getPosHeight(){return posHeight;}
    //GLfloat getNegHeight(){return negHeight;}
    void setHeight(GLfloat in){ height=in; }
    GLfloat getHeight(){ return height;}
    GLfloat getXRotation(){return xRotation;}
    GLfloat getYRotation(){return yRotation;}
    GLfloat getZRotation(){return zRotation;}
private:
    GLfloat height, xRotation, yRotation, zRotation;
};

#endif // SHAPE_H
