#include "camera.h"

void Camera::initialize(Point pos, Point look){
  camPos = pos;
  camLook = look;
  camPhi = D_ANGLE;
  recomputeOrientation();
}

void Camera::recomputeOrientation() {
  Vector radius = camPos - camLook;
  camR = radius.mag();
  camDir = Vector( radius.getX(), radius.getY(), radius.getZ() );
  camDir.normalize();
  glutPostRedisplay();
}

void Camera::gluLook(){
  gluLookAt(  
  camPos.getX(),  camPos.getY(),  camPos.getZ(),
  camLook.getX(), camLook.getY(), camLook.getZ(),
  0,              1,              0      );
}

void Camera::arcBall(){
  camPos = camLook + camR*Point(
    -sinf(camTheta)*sinf(camPhi),
    cosf(camPhi),
    -cosf(camTheta)*sinf(camPhi)
    );
}

void Camera::smooth(Point newPos, Point newLook){
  step = 0;
  vecPos = newPos - camPos;
  vecLook = newLook - camLook;
  vecPos /= STEP_SIZE;
  vecLook /= STEP_SIZE;
  camPhi = 0;
  camTheta = 0;
}

void Camera::update(){
  if(step < STEP_SIZE){
    camPos += vecPos;
    camLook += vecLook;
    step++;
  }
  recomputeOrientation();
}

