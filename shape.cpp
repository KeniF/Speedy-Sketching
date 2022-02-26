#include "shape.h"
#include <iostream>

using namespace std;
Shape::Shape(GLfloat xRotation, GLfloat yRotation, GLfloat zRotation)
{
    this->xRotation=xRotation;
    this->yRotation=yRotation;
    this->zRotation=zRotation;
}

Shape::Shape(){}

Shape::~Shape(){
    //cout<<"Shape deconstructor"<<endl;
}

void Shape::draw(GLUquadricObj * quadric, bool selected) {
  // shouldn't happen
}
