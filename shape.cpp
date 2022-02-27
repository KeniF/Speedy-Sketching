#include "shape.h"
#include <iostream>

using namespace std;
Shape::Shape(GLfloat xRotation, GLfloat yRotation, GLfloat zRotation):
    xRotation(xRotation), yRotation(yRotation), zRotation(zRotation)
{}

Shape::~Shape(){
    //cout<<"Shape deconstructor"<<endl;
}
