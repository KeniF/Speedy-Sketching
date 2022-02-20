#include "cone.h"

Cone::Cone(Point * centre, GLfloat base, GLfloat height,GLfloat xRotation, GLfloat yRotation, GLfloat  zRotation)
        :Shape(xRotation, yRotation, zRotation){
    this->centre=centre;
    this->base=base;
    this->height=height;
    type=Object::CONE;
}

Cone::~Cone(){
    delete centre;
}
