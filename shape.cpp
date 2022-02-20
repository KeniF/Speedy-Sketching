#include "shape.h"
#include <iostream>
using namespace std;
Shape::Shape(GLfloat xRotation, GLfloat yRotation, GLfloat zRotation):Object()
{
    this->xRotation=xRotation;
    this->yRotation=yRotation;
    this->zRotation=zRotation;
}

Shape::Shape(){}

Shape::~Shape(){
    //cout<<"Shape deconstructor"<<endl;
}
