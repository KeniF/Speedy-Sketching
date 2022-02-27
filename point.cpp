#include "point.h"

Point::Point(GLfloat x, GLfloat y, GLfloat z){
    //cout<<"Point constructor"<<endl;
    this->x=x;
    this->y=y;
    this->z=z;
    next=0; //important to initialise to 0!!
}

Point::Point(const Point & rhs){
    x=rhs.getX();
    y=rhs.getY();
    z=rhs.getZ();
    //next=new Point;
    last=0;
    next=0;
}

Point::~Point(){
    //cout<<"~Point"<<endl;
    delete next;
    next=0;
    last=0;
}//~Point

string Point::toString(){
    ostringstream stream;
    stream<<"<Point>"<<x<<","<<y<<","<<z;
    return stream.str();
}
