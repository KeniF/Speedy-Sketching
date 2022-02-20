#ifndef OBJECT_H
#define OBJECT_H
#include <string>
#include <sstream>
using namespace std;

class Object
{
public:
    Object();
    virtual ~Object();
    virtual string toString();
    virtual int getType();
    static const int NONE=0, FREEHAND=1, CIRCLE=6, TRIANGLE=11,
    RECTANGLE=16, CONE=21, CUBE=26, CYLINDER=31,SPHERE=41;
protected:
    int type;

};

#endif // OBJECT_H
