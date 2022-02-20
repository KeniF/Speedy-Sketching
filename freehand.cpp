#include "freehand.h"

Freehand::Freehand(bool closed, Line *line, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation):
        Shape(xRotation,yRotation,zRotation){
    this->closed=closed;
    this->line=line;
    type=FREEHAND;
}

Freehand::~Freehand(){
    delete line;
    line=0;
}
