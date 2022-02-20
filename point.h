#ifndef POINT_H
#define POINT_H
#include <QGLWidget>
#include <string>
#include <sstream>

using namespace std;
class Point{
public:
    Point(GLfloat x, GLfloat y, GLfloat z);
    //Point();
    Point (const Point &);
    ~Point();
    void setNext(Point *next){ this->next=next;}
    Point * getNext(Point *next){ return this->next;}
    Point * getLast(Point *last){return this->last;}
    void setLast(Point *last){this->last=last;}
    GLfloat Point::getX() const {return x;}
    GLfloat Point::getY() const {return y;}
    GLfloat Point::getZ() const {return z;}
    Point* next, * last;
    string toString();
private:
    GLfloat x,y,z;
};

#endif // POINT_H
