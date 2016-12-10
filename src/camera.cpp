#include "camera.h"

void Camera::recomputeOrientation() {
  // lookAtX = focus->getX();
  // lookAtY = focus->getY();
  // lookAtZ = focus->getZ();

  // dirX = radius*sin(theta)*sin(phi);
  // dirY = -radius*cos(phi);
  // dirZ = -radius*cos(theta)*sin(phi);

  // x = dirX + focus->getX();
  // y = dirY + focus->getY();
  // z = dirZ + focus->getZ();

  Vector radius = camPos - camLook;
  camR = radius.mag();
  // camDir = Vector( radius.getX(), radius.getY(), radius.getZ() );
  camDir = Vector(  camR*sinf(camTheta)*sinf(camPhi),
                    -camR*cosf(camTheta),
                    -camR*cosf(camPhi)*sinf(camPhi));

  // camPos = Point( camDir.getX() + camLook.getX(), camDir.getY() + camLook.getY(), camDir.getZ() + camDir.getZ());

  // camDir.normalize();
  radius.normalize();
  camPhi = dot( Vector(-1,0,0), radius );
  camTheta = dot( Vector(0,1,0), radius);
  
  // glutPostRedisplay();
}

void Camera::gluLook(){
  gluLookAt(  
  camPos.getX(),  camPos.getY(),  camPos.getZ(),
  camLook.getX(), camLook.getY(), camLook.getZ(),
  0,              1,              0      );
}

void Camera::arcBall(){
  //Move around the camera as our mouse moves
  camTheta -= (vecPos.getX() - mouseX) * 0.005;
  camPhi -= (vecPos.getY()- mouseY) * 0.005;

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

