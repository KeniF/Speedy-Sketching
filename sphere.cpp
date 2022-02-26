#include "sphere.h"

Sphere::Sphere(Point * centre, GLfloat radius, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation):
        Shape(xRotation,yRotation,zRotation){ // sphere, same in all directions
    this->centre=centre;
    this->radius=radius;
}

Sphere::~Sphere(){
    delete centre;
    centre=0;
}
void Sphere::draw(GLUquadricObj *quadric, bool selected) {
    if (selected) glColor4f(0.6,0.6,0.9,1.0);
    else glColor4f(0.5,0.5,0.8,1.0);
    gluQuadricNormals(quadric,GLU_NONE);
    gluQuadricDrawStyle(quadric,GLU_FILL);
    glRotatef(xRotation,1.0,0.0,0.0);
    glRotatef(yRotation,0.0,1.0,0.0);
    glTranslatef(centre->getX(),centre->getY(),centre->getZ());
    gluSphere(quadric,radius,sliceInSphere,sliceInSphere);
}
