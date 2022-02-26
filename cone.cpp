#include "cone.h"

Cone::Cone(Point * centre, GLfloat base, GLfloat height,GLfloat xRotation, GLfloat yRotation, GLfloat  zRotation)
        :Shape(xRotation, yRotation, zRotation){
    this->centre=centre;
    this->base=base;
    this->height=height;
}

Cone::~Cone(){
    delete centre;
}

void Cone::draw(GLUquadricObj *quadric, bool selected) {
    if(selected) glColor4f(0.8f,0.0,0.8,1.0);
    else glColor4f(0.7,0,0.7,1.0);

    glRotatef(xRotation,1.0,0.0,0.0);
    glRotatef(yRotation,0.0,1.0,0.0);
    glTranslatef(centre->getX(),centre->getY(),centre->getZ());
    glRotatef(zRotation,0,0,1.0f);
    glRotatef(-90.0f,1,0,0);
    gluCylinder(quadric,base/2.0f,0,height,sliceInCircle,sliceInCircle);
}
