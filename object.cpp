#include "object.h"
#include <iostream>
using namespace std;
Object::Object(){
    type=NONE;
}

Object::~Object(){
    //cout<<"Object deconstructor"<<endl;
}

string Object::toString(){
    return "Object";
}

int Object::getType(){
    return type;
}
