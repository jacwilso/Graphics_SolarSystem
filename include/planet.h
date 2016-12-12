#ifndef PLANET_H
#define PLANET_H

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

#include "Point.h"
#include "Vector.h"
#include "PlanetLine.h"
#include "Fireball.h"
using namespace std;

enum PLANET{SUN,MERCURY,VENUS,EARTH,MARS,JUPITER,SATURN,URANUS,NEPTUNE,COMET};
const float EARTH_MASS = 5.98 * pow(10, 24);
const float EARTH_RADIUS = 6371.0; // km
const float EARTH_TOSUN = 149600; // Mm
const float SUN_RADIUS = 109; // Earth radii

const int lineLife = 20;
const float lineSpawn = 10;
const float fireLife = 0.3;
const float fireSpawn = 1000;
class Planet{
private:
  float rotation;
  void drawSphere();
public:
  PLANET type;
  Point position;
  Vector orbitalVel;
  float radius, mass, toSun, axisTilt, rotationVel, orbitVel, theta, lastTime; 
  vector<Planet> satellites;
  GLuint shaderHandle, textureHandle, shaderTimeLoc, specularHandle, eggHandle;
  bool easterEgg;
  //static GLUquadric* obj;
  PlanetLine planetLine;
  bool lineOn;

  Fireball fireball;
  bool onFire;
  
  Planet(PLANET planet);
  Planet(Point position, float radius, float mass, float axisTilt, float rotationVel, Vector orbitalVel);
  void startTime();
  void draw();
  void update();
  void registerShader(GLuint handle, GLuint timeHandle) {shaderHandle = handle; shaderTimeLoc = timeHandle;}
  void registerTexture(GLuint handle) {textureHandle = handle;}
  void specularTexture(GLuint handle) {specularHandle = handle;}
  void setEggTexture(GLuint handle) {eggHandle = handle;}
};

#endif
