#include "rect.h"

Rect::Rect(Point * centre, GLfloat width, GLfloat length,GLfloat xRotation, GLfloat yRotation, GLfloat  zRotation)
        :Shape(xRotation,yRotation,zRotation){
    this->centre=centre;
    this->length=length;
    this->width=width;
    type=Object::RECTANGLE;
}

Rect::~Rect(){
    delete centre;
}
