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
    //if(age < 0.5) return;
    
    glPushMatrix();
        glTranslatef(location.getX(), location.getY(), location.getZ());
    	glutSolidSphere(radius, 10,10);
    glPopMatrix();
    
}