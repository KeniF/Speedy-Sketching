#include "triangle.h"

Triangle::Triangle(Point * p1, Point * p2, Point * p3,GLfloat xRotation, GLfloat yRotation, GLfloat zRotation):Shape(xRotation,yRotation,zRotation){
    pPoint1=p1;
    pPoint2=p2;
    pPoint3=p3;
    type=Object::TRIANGLE;
}

Triangle::~Triangle(){
    delete pPoint1;
    pPoint1=0;
    delete pPoint2;
    pPoint2=0;
    delete pPoint3;
    pPoint3=0;
}
