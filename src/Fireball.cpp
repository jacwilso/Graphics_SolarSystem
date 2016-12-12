#include "Fireball.h"


Fireball::Fireball(Point location, double lifespan, double radius, int spawnRate){
	this->location = location;
	this->lineRadius = radius;
	this->lifespan = lifespan;
	this->spawnRate = (double)spawnRate * TIME_CONSTANT; //convert to spawns per frame
	this->spawnCounter = 0.0;
}

void Fireball::spawnParticle(){
	Particle* newParticle = new Particle();
	newParticle->radius = lineRadius/10.0;
	newParticle->setLifespan(lifespan);
	
	double spawnRadius = lineRadius * 0.9;
	double x = fmod((double)rand(),(spawnRadius*2.0)) - spawnRadius;
	double y = fmod((double)rand(),(spawnRadius*2.0)) - spawnRadius;
	double z = fmod((double)rand(),(spawnRadius*2.0)) - spawnRadius;

	newParticle->setLocation(Point(x,y,z));

	particles.push_back(newParticle);
};

void Fireball::updatePosition(Point position, Point lastPosition){
	//this->location = position;
	for(unsigned int i = 0; i < particles.size(); i++){
		Point lastLocation = particles[i]->getLocation();
		Point newLocation = lastLocation + (lastPosition - position);
		particles[i]->setLocation(newLocation);
	}
}

void Fireball::drawEmitter(){
	
}