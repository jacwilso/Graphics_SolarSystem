#include "ParticleSystem.h"

class Fireball : public ParticleSystem{
public:
	//(Emitter X, Y, Z), Lifespan, line radius, Spawn Rate
	Fireball(Point,	 double, 	double, 	 int);
	Fireball(){}

	//Implementations

	//Make a new particle from the fountain cone
	void spawnParticle();

	//Draw the fountain
	void drawEmitter();

	void updatePosition(Point,Point);

	void updateRadius(double newRadius){this->lineRadius = newRadius;}
private:

	//Fountain parameters
	double lifespan;
	double lineRadius;
};