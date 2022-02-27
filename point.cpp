#include "point.h"

Point::Point(GLfloat x, GLfloat y, GLfloat z):
    x(x), y(y), z(z), next(0)
{}

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
