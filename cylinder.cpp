#include "cylinder.h"

Cylinder::Cylinder(bool isTube, bool hasBase,Point * centre, GLfloat width, GLfloat length,GLfloat xRotation, GLfloat yRotation, GLfloat  zRotation)
        :Shape(xRotation,yRotation,zRotation){
    this->centre=centre;
    this->length=length;
    this->width=width;
    this->tube=isTube;
    this->hasBase=hasBase;
    type=Object::CYLINDER;
}

Cylinder::~Cylinder(){
    delete centre;
}
