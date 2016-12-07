#include "Planet.h"

//GLUquadric* Planet::obj = gluNewQuadric();
Planet::Planet(enum planet){
  //gluQuadricNormals(obj,GL_SMOOTH);
  switch(planet){
    case SUN:
      position = Point(toSun,0,0);
      radius = 109; // Earth radiuses
      mass = 333000; // Earth masses
      toSun = 0; // 
      axisTilt = 7.25; // angle
      rotationVel = 0; // ???
      satellites = NULL;
      break;
    case MERCURY:
      radius = 0.3829;
      mass = 0.055;
      toSun = ; // ???
      axisTilt = 2.04;
      rotationVel = 10.892; // km/h
      satellites = NULL;
      position = Point(toSun,0,0);
      break;
    case VENUS:
      radius = 0.9499;
      mass = 0.815;
      toSun = ; // ???
      axisTilt = 177.36;
      rotationVel = 6.52;
      satellites = NULL;
      position = Point(toSun,0,0);
      break;
    case EARTH:
      radius = 1;
      mass = 1;
      toSun = ; // ???
      axisTilt = 23.4392811;
      rotationVel = 1674.4;
      satellites.push_back(COMET);
      position = Point(toSun,0,0);
      break;
    case MARS:
      radius = 0.533;
      mass = 0.107;
      toSun = ;
      axisTilt = 25.19;
      rotationVel = 868.22;
      satellites = NULL; // 2
      position = Point(toSun,0,0);
      break;
    case JUPITER:
      radius = 11.209;
      mass = 317.8;
      toSun = ;
      axisTilt = 3.13;
      rotationVel = 12.6;
      satellites = NULL; // 67!
      position = Point(toSun,0,0);
      break;
    case SATURN:
      radius = 9.4492;
      mass = 95.159;
      toSun = ;
      axisTilt = 26.73;
      rotationVel = 9.87;
      satellites = NULL; // 62
      position = Point(toSun,0,0);
      break;
    case URANUS:
      radius = 4.007;
      mass = 14.536;
      toSun = ;
      axisTilt = 97.77;
      rotationVel = 2.59;
      satellites = NULL; // 27
      position = Point(toSun,0,0);
      break;
    case NEPTUNE:
      radius = 3.883;
      mass = 17.147;
      toSun = ;
      axisTilt = 28.32;
      rotationVel = 2.68;
      satellites = NULL; // 14
      position = Point(toSun,0,0);
      break;
    case COMET:
      radius = 0;
      mass = 0;
      toSun = 0;
      axisTilt = 0;
      rotationVel = 0;
      satellites = NULL;
      position = Point(toSun,0,0);
      break;
  }
}

void Planet::draw(){
  glPushMatrix();
    glTranslatef(position.getX(), position.getY(), position.getZ());
    glRotatef(axisTilt, 0, 1, 0);
    glutSolidSphere(raidus, 32, 32);
  glPopMatrix();
}

void Planet::update(){
  
}
