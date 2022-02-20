#include "circle.h"
#include "iostream"
using namespace std;
Circle::Circle(Point * centre, GLfloat radius, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation): Shape(xRotation,yRotation,zRotation){
    this->centre=centre;
    this->radius=radius;
    type=Object::CIRCLE;
}

Circle::~Circle(){
    //cout<<"Circle deconstructor"<<endl;
    delete centre;
    centre=0;
}

string Circle::toString(){
    ostringstream stream;
    stream<<"<Circle> Radius:"<<radius<<"Centre: "<<centre->toString()<<endl;
    string output=stream.str();
    return output;
}
