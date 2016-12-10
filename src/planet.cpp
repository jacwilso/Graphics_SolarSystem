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
      radius = SUN_RADIUS; // Earth radiuses
      mass = 333000; // Earth masses
      toSun = 0; // Mm
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
  radius *= EARTH_RADIUS;
  mass *= EARTH_MASS;
  position = Point(type==SUN ? 0: (toSun+radius+SUN_RADIUS*EARTH_RADIUS), 0.0, 0.0);
  //cout<<"[radius] "<<radius<<endl;
  //cout<<"[toSun] "<<toSun<<endl;
}

void Planet::startTime(){
  lastTime = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  for(unsigned int i=0; i<satellites.size(); i++)
    satellites[i].lastTime = lastTime;
}

void Planet::draw(){
  glPushMatrix();
    //glTranslatef(position.getX(), position.getY(), position.getZ());
    glRotatef(axisTilt, 1.0, 0.0, 0.0);
    
    glDisable(GL_LIGHTING);
      glBegin(GL_LINES);
        glVertex3f(0, -3*radius/(EARTH_RADIUS*2.0), 0);
        glVertex3f(0, 3*radius/(EARTH_RADIUS*2.0), 0);
      glEnd();
    glEnable(GL_LIGHTING);

    glPushMatrix();
      glRotatef(rotation, 0.0, 1, 0.0);
      // glutSolidSphere(radius/EARTH_RADIUS, 32, 32);
      drawSphere();
      
      // glPushMatrix();
      //   glTranslatef(radius, 0, 0);
      //   glutSolidSphere(radius/(EARTH_RADIUS*10.0), 32, 32);
      // glPopMatrix();
    
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

void Planet::drawSphere() {
  float stacks = 32, slices = 32;
  float thetaStep = 6.28f / stacks;
  float phiStep = 6.28f / slices;
  float partialRadius = radius/EARTH_RADIUS;

  glPushMatrix(); {
      glTranslatef( 0, partialRadius, 0 );
      
      for( float theta = 0; theta < 6.28; theta += thetaStep ) {
          for( float phi = -3.14; phi < 3.14; phi += phiStep ) {
              Point p1( partialRadius * cos(theta) * sin(phi),
                        partialRadius * cos(phi),
                        partialRadius * sin(theta) * sin(phi) );
              Point p2( partialRadius * cos(theta+thetaStep) * sin(phi),
                        partialRadius * cos(phi),
                        partialRadius * sin(theta+thetaStep) * sin(phi) );
              Point p3( partialRadius * cos(theta) * sin(phi+phiStep),
                        partialRadius * cos(phi+phiStep),
                        partialRadius * sin(theta) * sin(phi+phiStep) );
              Point p4( partialRadius * cos(theta+thetaStep) * sin(phi+phiStep),
                        partialRadius * cos(phi+phiStep),
                        partialRadius * sin(theta+thetaStep) * sin(phi+phiStep) );
          
              Vector n1 = p1 - Point(0,0,0);
              Vector n2 = p2 - Point(0,0,0);
              Vector n3 = p3 - Point(0,0,0);
              Vector n4 = p4 - Point(0,0,0);
          
              glBegin( GL_QUADS ); {
                  glTexCoord2f( theta / 6.28, (phi+3.14f) / 6.28 );
                  n1.glNormal();
                  p1.glVertex();
              
                  glTexCoord2f( (theta+thetaStep) / 6.28, (phi+3.14f) / 6.28 );
                  n2.glNormal();
                  p2.glVertex();
              
                  glTexCoord2f( (theta+thetaStep) / 6.28, ((phi+phiStep)+3.14f) / 6.28 );
                  n4.glNormal();
                  p4.glVertex();
              
                  glTexCoord2f( theta / 6.28, ((phi+phiStep)+3.14f) / 6.28 );
                  n3.glNormal();
                  p3.glVertex();
              } glEnd();
          }
      }
  }; glPopMatrix();
}

void Planet::update(){
  float currTime = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  float timeDiff = currTime - lastTime;
  lastTime = currTime;
  rotation += rotationVel / (2*M_PI*radius) * (180.0 / M_PI ) * timeDiff * 60;
  //cout<<orbitalVel.getX()<<" "<<orbitalVel.getY()<<" "<<orbitalVel.getZ()<<endl;
  //position = Point( position.getX() + timeDiff*orbitalVel.getX(), position.getY() + timeDiff*orbitalVel.getY(), position.getZ() + timeDiff*orbitalVel.getZ() ); 
  for(unsigned int i=0; i<satellites.size(); i++)
    satellites[i].update();
}
