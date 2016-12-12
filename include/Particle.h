
#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include "Point.h"
#include "Vector.h"

using namespace std;

class Particle{
public:
	Particle();

	Particle(Point,Vector);

	void draw();


	//Getters and Setters
	Vector getVelocity(){return velocity;}
	Point getLocation(){return location;}
	double getAge(){return age;}
	double getLifespan(){return lifespan;}

	void setVelocity(Vector newVelocity){this->velocity = newVelocity;}
	void setLocation(Point newLocation){this->location = newLocation;}
	void setAge(double newAge){this->age = newAge;}
	void setLifespan(double newLifespan){this->lifespan = newLifespan;}

private:
	Point location;
	Vector velocity;
	double age;
	double lifespan;
};