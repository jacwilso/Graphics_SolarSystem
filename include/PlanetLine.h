#ifndef PLANETLINE_H
#define PLANETLINE_H
#include "ParticleSystem.h"

class PlanetLine : public ParticleSystem{
public:
	//(Emitter X, Y, Z), Lifespan, line radius, Spawn Rate
	PlanetLine(Point,	 double, 	double, 	 int);
	PlanetLine(){}

	//Implementations

	//Make a new particle from the fountain cone
	void spawnParticle();

	//Draw the fountain
	void drawEmitter();

	void updatePosition(Point,Point);


private:

	//Fountain parameters
	double lifespan;
	double lineRadius;
};

#endif
