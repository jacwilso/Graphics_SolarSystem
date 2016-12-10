#ifndef CAMERA_H
#define CAMERA_H

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
#include <iostream>

#include "Point.h"
#include "Vector.h"
using namespace std;

const float D_ANGLE = 0.005; 
const int STEP_SIZE = 40;
class Camera{
  private:
    float camR, camTheta, camPhi;
    int step;
    Vector vecPos, vecLook;
  public: 
    Point camPos, camLook;
    Vector camDir;
    float mouseX,mouseY;

    void recomputeOrientation();

    void setCamR(float x,float y){
    //if(-200 < camR && camR < 200) camR = 200*ceil(camR/200.0); else
     camR += D_ANGLE*(x-mouseX) + D_ANGLE*(y-mouseY);}
    void setCamAngle(float x, float y){
      camTheta += (x-mouseX)*D_ANGLE;
      camPhi += (mouseY-y)*D_ANGLE;
      if(camPhi <= 0) camPhi = D_ANGLE;
      if(camPhi >= M_PI) camPhi = M_PI-D_ANGLE;}

    void gluLook();
    void arcBall();
    void smooth(Point newPos, Point newLook);
    void update();
};

#endif
