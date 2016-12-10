#include "camera.h"

void Camera::recomputeOrientation() {
  Vector radius = camPos - camLook;
  camR = radius.mag();
  camDir = Vector( radius.getX(), radius.getY(), radius.getZ() );

  camDir.normalize();
  radius.normalize();
  camPhi = dot( Vector(-1,0,0), radius );
  camTheta = dot( Vector(0,1,0), radius);
  /*camDir = Vector(  sinf(cameraTheta)*sinf(cameraPhi),
                    -cosf(cameraTheta)*sinf(cameraPhi),
                    -cosf(cameraPhi) );*/
  glutPostRedisplay();
}

void Camera::gluLook(){
  gluLookAt(  
  camPos.getX(),  camPos.getY(),  camPos.getZ(),
  camLook.getX(), camLook.getY(), camLook.getZ(),
  0,              1,              0      );
}

void Camera::arcBall(){
  camDir = Vector(  sinf(camTheta)*sinf(camPhi),
    -cosf(camPhi),
    -cosf(camTheta)*sinf(camPhi));
  camPos = camLook + camR*camDir;
}

void Camera::smooth(Point newPos, Point newLook){
  step = 0;
  vecPos = newPos - camPos;
  vecLook = newLook - camLook;
  vecPos /= STEP_SIZE;
  vecLook /= STEP_SIZE;
}

void Camera::update(){
  if(step < STEP_SIZE){
    camPos += vecPos;
    camLook += vecLook;
    step++;
  }
  recomputeOrientation();
}
