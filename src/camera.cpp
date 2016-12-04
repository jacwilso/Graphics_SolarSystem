#include "camera.h"

// variables for the look at matrix
void Camera::setCamera(float cameraX,float cameraY,float cameraZ,float cameraTheta,float cameraPhi,float cameraR,float lookAtX,float lookAtY,float lookAtZ){
  this->cameraX=cameraX;
  this->cameraY=cameraY;
  this->cameraZ=cameraZ;
  this->cameraTheta=cameraTheta;
  this->cameraPhi=cameraPhi;
  this->cameraR=cameraR;
  this->lookAtX=lookAtX;
  this->lookAtY=lookAtY;
  this->lookAtZ=lookAtZ;
  recomputeOrientation();
}

// recompute the orientation
void Camera::recomputeOrientation() {
  dirX=sinf(cameraTheta)*sinf(cameraPhi);
  dirZ=-cosf(cameraTheta)*sinf(cameraPhi);
  dirY=-cosf(cameraPhi);
  float mag = sqrt( dirX*dirX + dirY*dirY + dirZ*dirZ );
  dirX /= mag;
  dirY /= mag;
  dirZ /= mag;

  glutPostRedisplay();
}

// arc ball look at matrix
void Camera::arcBall(){
    gluLookAt(  cameraR*dirX+lookAtX,  -cameraR*dirY+lookAtY,  -cameraR*dirZ+lookAtZ, 
                lookAtX,  lookAtY,  lookAtZ,
                0,        1,        0      );
}

// free cam look at matrix
void Camera::freeCam(){
    gluLookAt(  cameraX, cameraY, cameraZ,
                cameraX+dirX, cameraY+dirY, cameraZ+dirZ,
                0.0f,1.0f,0.0f);
}

// first person look at matrix -- need orientation of object
void Camera::firstPerson(float objTheta){
      gluLookAt(  lookAtX,  lookAtY+4,  lookAtZ, 
                lookAtX-4*cos(objTheta*DEG_RAD),  lookAtY+4,  lookAtZ+4*sin(objTheta*DEG_RAD),
                0,                                1,        0);
}

// sky camera look at matrix -- need orientation of object
void Camera::skyCam(float objTheta){
      gluLookAt(  lookAtX+4*cos(objTheta*DEG_RAD)+.001,  lookAtY+cameraR,  lookAtZ+.001-4*sin(objTheta*DEG_RAD),
                lookAtX,  lookAtY,  lookAtZ,
                0,                                1,        0);
}
