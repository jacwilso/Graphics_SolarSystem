#include "PlanetLine.h"


PlanetLine::PlanetLine(Point location, double lifespan, double radius, int spawnRate){
	this->location = location;
	this->lineRadius = radius;
	this->lifespan = lifespan;
	this->spawnRate = (double)spawnRate * TIME_CONSTANT; //convert to spawns per frame
	this->spawnCounter = 0.0;
}

void PlanetLine::spawnParticle(){
	Particle* newParticle = new Particle();
	newParticle->radius = lineRadius;
	newParticle->setLifespan(lifespan);
	//newParticle->setLocation(location);

	particles.push_back(newParticle);
};

void PlanetLine::updatePosition(Point position, Point lastPosition){
	//this->location = position;
	for(unsigned int i = 0; i < particles.size(); i++){
		Point lastLocation = particles[i]->getLocation();
		Point newLocation = lastLocation + (lastPosition - position);
		particles[i]->setLocation(newLocation);
	}
}

void PlanetLine::drawEmitter(){
	
}