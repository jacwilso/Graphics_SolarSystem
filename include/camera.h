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
using namespace std;

class Camera{
  private:
    const static float move=0.6; // distance the camera moves forward
    const static float D_ANGLE = 0.005; // angle modifier for click + drag
    const static float DEG_RAD=3.14159/180; // degrees to radian
    float cameraX,cameraY,cameraZ; // camera x,y,z position
    float cameraTheta,cameraPhi,cameraR; // camera theta, phi, and radius
    float dirX,dirY,dirZ; // x,y,z looking direction
    float lookAtX,lookAtY,lookAtZ; // point to look at x,y,z
    float mouseX,mouseY; // mouse x and y

  public: 
    void setCamera(float cameraX,float cameraY,float cameraZ,float cameraTheta,float cameraPhi,float cameraR,float lookAtX,float lookAtY,float lookAtZ); // sets the camera variables
  
    void recomputeOrientation(); // recomputes the orientation

    // get the x,y,z directions
    float getDirX() 
    {return dirX;}
    float getDirY()
    {return dirY;}
    float getDirZ()
    {return dirZ;}

    // sets the x and y mouse buttons
    void setMouse(float mouseX,float mouseY)
    {this->mouseX=mouseX;
     this->mouseY=mouseY;}
    // sets the look at point
    void setLookAt(float lookAtX,float lookAtY,float lookAtZ)
    {this->lookAtX=lookAtX;
     this->lookAtY=lookAtY;
     this->lookAtZ=lookAtZ;
    }

    // increments the camera position -- moves forward
    void incCam()
    {cameraX+=dirX*move;
     cameraY+=dirY*move;
     cameraZ+=dirZ*move;}
    // decrements the camera position -- moves back
    void decCam()
    {cameraX-=dirX*move;
     cameraY-=dirY*move;
     cameraZ-=dirZ*move;}

    // set the angle of the camera from change of mouse position
    void setCamAngle(float x,float y)
    {cameraTheta+=(x-mouseX)*D_ANGLE;
     cameraPhi+=(mouseY-y)*D_ANGLE;
     if(cameraPhi<=0) cameraPhi=D_ANGLE;
     if(cameraPhi>=M_PI) cameraPhi=M_PI-D_ANGLE;
    }
    // set the camera radius based on mouse position
    void setCamR(float x,float y)
    {cameraR += (x-mouseX)+(y-mouseY);
    }

    void arcBall(); // arc ball camera
    void freeCam(); // free camera
    void firstPerson(float objTheta); // first person camera
    void skyCam(float objTheta); // sky camera
};

#endif
