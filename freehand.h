#ifndef FREEHAND_H
#define FREEHAND_H
#include "shape.h"
#include "line.h"
class Freehand: public Shape{
    public:
        Freehand(bool closed, Line * line, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation);
        ~Freehand();
        bool isClosedShape(){return closed;}
        Line * getLine(){return line;}
    private:
        bool closed;
        Line * line;
};

#endif // FREEHAND_H
