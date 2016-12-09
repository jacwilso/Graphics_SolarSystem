#include <iostream>

#include "planet.h"
using namespace std;

//GLUquadric* Planet::obj = gluNewQuadric();
Planet::Planet(PLANET planet){
  //gluQuadricNormals(obj,GL_SMOOTH);
  type = planet;
  rotation = 0;
  switch(planet){
    case SUN:
      cout<<"SUN"<<endl;
      radius = SUN_RADIUSE; // Earth radiuses
      mass = 333000; // Earth masses
      toSun = -SUN_RADIUSE*EARTH_RADIUS; // Mm
      axisTilt = 7.25; // degrees
      rotationVel = 7.189*pow(10,3); // km/h 
      satellites.clear();
      break;
    case MERCURY:
      cout<<"MERCURY"<<endl;
      radius = 0.3829;
      mass = 0.055;
      toSun = 57910; 
      axisTilt = 2.04;
      rotationVel = 10.892; // km/h
      satellites.clear();
      break;
    case VENUS:
      cout<<"VENUS"<<endl;
      radius = 0.9499;
      mass = 0.815;
      toSun = 108200;
      axisTilt = 177.36;
      rotationVel = 6.52;
      satellites.clear();
      break;
    case EARTH:
      cout<<"EARTH"<<endl;
      radius = 1;
      mass = 1;
      toSun = EARTH_TOSUN; 
      axisTilt = 23.4392811;
      rotationVel = 1674.4;
      //satellites.push_back(COMET);
      break;
    case MARS:
      radius = 0.533;
      mass = 0.107;
      toSun = 227940;
      axisTilt = 25.19;
      rotationVel = 868.22;
      //for(int i=0; i<2; i++)
        //satellites.push_back(COMET);
      break;
    case JUPITER:
      radius = 11.209;
      mass = 317.8;
      toSun = 778330;
      axisTilt = 3.13;
      rotationVel = 12.6;
      //for(int i=0; i<67; i++)
        //satellites.push_back(COMET);
      break;
    case SATURN:
      cout<<"SATURN"<<endl;
      radius = 9.4492;
      mass = 95.159;
      toSun = 1424600;
      axisTilt = 26.73;
      rotationVel = 9.87;
      //for(int i=0; i<62; i++)
        //satellites.push_back(COMET);
      break;
    case URANUS:
      cout<<"URANUS"<<endl;
      radius = 4.007;
      mass = 14.536;
      toSun = 2873550;
      axisTilt = 97.77;
      rotationVel = 2.59;
      //for(int i=0; i<27; i++)
        //satellites.push_back(COMET);
      break;
    case NEPTUNE:
      cout<<"NEPTUNE"<<endl;
      radius = 3.883;
      mass = 17.147;
      toSun = 4501000;
      axisTilt = 28.32;
      rotationVel = 2.68;
      //for(int i=0; i<14; i++)
        //satellites.push_back(COMET);
      break;
    default:
      cout<<"DEFAULT / COMET"<<endl;
    //case COMET:
      radius = 0;
      mass = 0;
      toSun = 0;
      axisTilt = 0;
      rotationVel = 0;
      satellites.clear();
      break;
  }
  rotationVel /= EARTH_RADIUS;
  toSun /= EARTH_RADIUS;
  //toSun *= 1000;
  position = Point(toSun + SUN_RADIUSE, 0.0, 0.0);
  //cout<<"[radius] "<<radius<<endl;
  //cout<<"[toSun] "<<toSun<<endl;
}

void Planet::startTime(){
  lastTime = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  for(unsigned int i=0; i<satellites.size(); i++){
    satellites[i].lastTime = lastTime;
}

void Planet::draw(){
  glPushMatrix();
    //glTranslatef(position.getX(), position.getY(), position.getZ());
    glRotatef(axisTilt, 1.0, 0.0, 0.0);
    
    glDisable(GL_LIGHTING);
      glBegin(GL_LINES);
        glVertex3f(0, -3*radius/2.0, 0);
        glVertex3f(0, 3*radius/2.0, 0);
      glEnd();
    glEnable(GL_LIGHTING);

    glPushMatrix();
      glRotatef(rotation, 0.0, 1, 0.0);
      glutSolidSphere(radius, 32, 32);
      
      glPushMatrix();
        glTranslatef(radius, 0, 0);
        glutSolidSphere(radius/10.0, 32, 32);
      glPopMatrix();
    
    glPopMatrix();
  glPopMatrix();

  for(unsigned int i=0; i<satellites.size(); i++){
    glPushMatrix();
      glTranslatef( satellites[i].position.getX(), satellites[i].position.getY(), satellites[i].position.getZ()); 
      /*glTranslatef( position.getX() + satellites[i].position.getX(), 
                    position.getY() + satellites[i].position.getY(), 
                    position.getZ() + satellites[i].position.getZ()); */
      satellites[i].draw();
    glPopMatrix();
  }
}

void Planet::update(){
  float currTime = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  float timeDiff = currTime - lastTime;
  lastTime = currTime;
  rotation += rotationVel / (2*M_PI*radius) * (180.0 / M_PI ) * timeDiff * 60;
  for(unsigned int i=0; i<satellites.size(); i++)
    satellites[i].update();
}
