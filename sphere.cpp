#include "sphere.h"

Sphere::Sphere(Point * centre, GLfloat radius, GLfloat xRotation, GLfloat yRotation, GLfloat zRotation):
        Shape(xRotation,yRotation,zRotation){ // sphere, same in all directions
    this->centre=centre;
    this->radius=radius;
    type=SPHERE;
}

Sphere::~Sphere(){
    delete centre;
    centre=0;
}
