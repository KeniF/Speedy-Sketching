#ifndef LINE_H
#define LINE_H
#include <Point.h>
#include <shape.h>
#include <string>
#include <iostream>
#include <sstream>
class Line:public Shape
{
public:
    Line(Point *head,GLfloat xRotation,GLfloat yRotation,GLfloat zRotation);
    Line();
    Line(const Line &);
    virtual ~Line();
    void addPoint(Point *point);
    Point * getHeadPoint(){return pHead;}
    Point * getTail(){return pTail;}
    virtual string toString();
    int getTotalPoints(){return totalPoints;}
    GLfloat getMinX(){ return minX; }
    GLfloat getMaxX(){ return maxX;}
    GLfloat getMinY(){ return minY;}
    GLfloat getMaxY(){return maxY;}
    Line * next;
private:
    Point * pHead, * pTail;
    int totalPoints;
    GLfloat minX,maxX,minY,maxY;
};

#endif // LINE_H
