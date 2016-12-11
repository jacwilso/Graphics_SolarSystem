#ifndef SOLAR_SYSTEM_H
#define SOLAR_SYSTEM_H

#include <GL/glew.h>

#ifdef __APPLE__			// if compiling on Mac OS
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else					// else compiling on Linux OS
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <math.h>
#include <map>

#include "Point.h"
#include "planet.h"
using namespace std;

const double G = 6.67384*pow(10,-11); // m^3 / (kg * s^2 )
class Solar_System{
public:
  Point position;
  vector<Planet*> solar_sys;
  vector<bool> pick;
  double xj, yj, zj; 
  map<int, double> xDiff, yDiff, zDiff;
  
  Solar_System();
  void calculate();
  void startTime();
  void update();
  void draw();
  void cleanup();
  void setShader(GLuint, GLuint, PLANET);
  void setTexture(GLuint, PLANET);
  void newPlanet(Point position);
  void specularTexture(GLuint, PLANET);
  void setEggTexture(GLuint);
  void easterEgg();
};

#endif
