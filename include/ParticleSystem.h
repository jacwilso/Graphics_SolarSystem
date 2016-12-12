#include <GL/glew.h>       // OpenGL Extension Wrangler

#include "Particle.h"
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <algorithm>
#define TIME_CONSTANT 0.017
#define M_PI  3.14159265358979323846  /* pi */


class ParticleSystem{
public:
	
	//Update the particles in the system
	void update();

	//Draw the system and its particles
	void draw(int);

	//Calculate the overall force from the system on the particles
	void calculateNetForce();

	//Print how many particles you have, net force, etc.
	void printStats();

	//Set the shader to be applied to the particles
	void setShader(GLuint);

	void clearParticles();

	//INTERFACES
	virtual void spawnParticle() = 0;
	virtual void drawEmitter() = 0;

protected:
	//System location
	Point location;

	//List of particles
	vector<Particle*> particles;
	
	//Overall force, and the list of forces in the system
	Vector netForce;
	vector<Vector> forces;

	//Spawning properties
	double spawnRate;
	double spawnCounter;

	//Reference to the shader program
	GLuint shaderProgramHandle;

	//Uniforms to pass particle age info to the shader
	GLuint uniformAgeLoc;
	GLuint uniformLifeLoc;

	
};