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
using namespace std;

enum PLANET{SUN,MERCURY,VENUS,EARTH,MARS,JUPITER,SATURN,URANUS,NEPTUNE,COMET};
const float EARTH_MASS = 5.98 * pow(10, 24);
const float EARTH_RADIUS = 6371.0; // km
const float EARTH_TOSUN = 149600; // Mm
const float SUN_RADIUS = 109; // Earth radii
class Planet{
private:
  float rotation;
  void drawSphere();
public:
  PLANET type;
  Point position;
  Vector orbitalVel;
  float radius, mass, toSun, axisTilt, rotationVel, lastTime; 
  vector<Planet> satellites;
  GLuint shaderHandle, textureHandle, shaderTimeLoc, specularHandle;
  //static GLUquadric* obj;
  
  Planet(PLANET planet);
  Planet(Point position, float radius, float mass, float toSun, float axisTilt, float rotationVel, Vector orbitalVel);
  void startTime();
  void draw();
  void update();
  void registerShader(GLuint handle, GLuint timeHandle) {shaderHandle = handle; shaderTimeLoc = timeHandle;}
  void registerTexture(GLuint handle) {textureHandle = handle;}
  void specularTexture(GLuint handle) {specularHandle = handle;}
};

#endif
