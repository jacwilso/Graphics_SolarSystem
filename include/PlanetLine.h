#include "ParticleSystem.h"

class PlanetLine : public ParticleSystem{
public:
	//(Emitter X, Y, Z), Lifespan, Spawn Rate
	PlanetLine(Point,	 double, 	 int);
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
};