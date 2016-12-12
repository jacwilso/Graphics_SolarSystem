#include "Particle.h"



Particle::Particle(){
    this->age = 0.0;
}


Particle::Particle(Point p, Vector v){
    this->age = 0.0;
	this->location = p;
	this->velocity = v;

}

void Particle::draw(){
    glDisable(GL_LIGHTING);
    glPushMatrix();
        glColor3f(1.0,1.0,1.0);
        glTranslatef(location.getX(), location.getY(), location.getZ());
    	glutSolidCube(1);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}