#include <iostream>

#include "planet.h"
using namespace std;

//GLUquadric* Planet::obj = gluNewQuadric();
Planet::Planet(PLANET planet){
  //gluQuadricNormals(obj,GL_SMOOTH);
  type = planet;
  rotation = 0;
  theta = 0;
  switch(planet){
    case SUN:
      cout<<"SUN"<<endl;
      radius = SUN_RADIUS; // Earth radiuses
      mass = 333000; // Earth masses
      toSun = 0; // Mm
      axisTilt = 7.25; // degrees
      rotationVel = 7.189*pow(10,3); // km/h 
      orbitVel = 0; // km/s
      satellites.clear();
      break;
    case MERCURY:
      cout<<"MERCURY"<<endl;
      radius = 0.3829;
      mass = 0.055;
      toSun = 57910; 
      axisTilt = 2.04;
      rotationVel = 10.892; // km/h
      orbitVel = 47.87;
      satellites.clear();
      break;
    case VENUS:
      cout<<"VENUS"<<endl;
      radius = 0.9499;
      mass = 0.815;
      toSun = 108200;
      axisTilt = 177.36;
      rotationVel = 6.52;
      orbitVel = 35.02;
      satellites.clear();
      break;
    case EARTH:
      cout<<"EARTH"<<endl;
      radius = 1;
      mass = 1;
      toSun = EARTH_TOSUN; 
      axisTilt = 23.4392811;
      rotationVel = 1674.4;
      orbitVel = 29.78;
      //satellites.push_back(COMET);
      break;
    case MARS:
      radius = 0.533;
      mass = 0.107;
      toSun = 227940;
      axisTilt = 25.19;
      rotationVel = 868.22;
      orbitVel = 24.077;
      //for(int i=0; i<2; i++)
        //satellites.push_back(COMET);
      break;
    case JUPITER:
      cout<<"JUPITER"<<endl;
      radius = 11.209;
      mass = 317.8;
      toSun = 778330;
      axisTilt = 3.13;
      rotationVel = 12.6;
      orbitVel = 13.07;
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
      orbitVel = 9.69;
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
      orbitVel = 6.81;
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
      orbitVel = 5.43;
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
      orbitVel = 0;
      satellites.clear();
      break;
  }
  radius *= EARTH_RADIUS;
  mass *= EARTH_MASS;
  position = Point(type==SUN ? 0: (toSun+radius+SUN_RADIUS*EARTH_RADIUS), 0.0, 0.0);
  planetLine = PlanetLine(position, lineLife +(100 * (50.0-orbitVel)/orbitVel), radius/EARTH_RADIUS/2.0, lineSpawn * (orbitVel/50.0));
  fireball = Fireball(position, fireLife, radius/EARTH_RADIUS, fireSpawn);
  lineOn = false;
  onFire = false;

  shaderHandle = 0;
  textureHandle = 0;
  specularHandle = 0;
  easterEgg = false;
  //cout<<"[radius] "<<radius<<endl;
  //cout<<"[toSun] "<<toSun<<endl;
}

Planet::Planet(Point position, float radius, float mass, float axisTilt, float rotationVel, Vector orbitalVel){
  type = COMET;
  rotation = 0;
  theta = 0;
  this->radius = radius;
  this->mass = mass;
  this->toSun = (position - Point(0,0,0)).mag();
  this->axisTilt = axisTilt;
  this->rotationVel = rotationVel;
  this->orbitalVel = orbitalVel;
  radius *= EARTH_RADIUS;
  mass *= EARTH_MASS;
  shaderHandle = 0;
  textureHandle = 0;
  specularHandle = 0;
  easterEgg = false;
}

void Planet::startTime(){
  lastTime = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  for(unsigned int i=0; i<satellites.size(); i++)
    satellites[i].lastTime = lastTime;
}

void Planet::draw(){

  //Update our planet line to follow the planet
  planetLine.draw(0);
  fireball.draw(1);
  

  // specify each of our material component colors
  GLfloat diffCol[4] = { 1.0, 1.0, 1.0 };
  GLfloat specCol[4] = { 0.2, 0.2, 0.2 };
  if(type == SUN) {
    specCol[0] = 1.0;
    specCol[1] = 1.0;
    specCol[2] = 1.0;
  }
  GLfloat ambCol[4] = { 0, 0, 0 };
  
  // and now set them for the front and back faces
  glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffCol );
  glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specCol );
  glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 0.0 );   // as well as the shininess - this value ranges between 0 & 90
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambCol );
  glPushMatrix();
    //glTranslatef(position.getX(), position.getY(), position.getZ());
    glRotatef(axisTilt, 1.0, 0.0, 0.0);
    
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glUseProgram(0);
      glBegin(GL_LINES);
        glVertex3f(0, -3*radius/(EARTH_RADIUS*2.0), 0);
        glVertex3f(0, 3*radius/(EARTH_RADIUS*2.0), 0);
      glEnd();
    glEnable(GL_LIGHTING);

    glPushMatrix();
      glRotatef(rotation, 0.0, 1, 0.0);

      // enable shader
      glUseProgram(shaderHandle);
      // enable textures.
      glEnable( GL_TEXTURE_2D );
      glActiveTexture(GL_TEXTURE0);
      if(easterEgg)
        glBindTexture(GL_TEXTURE_2D, eggHandle);
      else
        glBindTexture(GL_TEXTURE_2D, textureHandle);
      if(specularHandle != 0) {
        // std::cout << "Ahhhhhh!" << std::endl;
        GLuint displacementMap_location = glGetUniformLocation(shaderHandle, "displacementMap");
        glUniform1i(displacementMap_location, 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularHandle);
      } 

      glUniform1f(shaderTimeLoc, glutGet(GLUT_ELAPSED_TIME)/1000.0f);

      GLUquadricObj *myQuad = gluNewQuadric();
      if(textureHandle != 0)
        gluQuadricTexture(myQuad, true);
      glRotatef(-90, 1, 0, 0);
      gluSphere(myQuad, radius/EARTH_RADIUS, 32, 32);
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

  glUseProgram(0);

  glActiveTexture(GL_TEXTURE0);
  glDisable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE1);
  glDisable( GL_TEXTURE_2D );
  glActiveTexture(GL_TEXTURE0);
}

void Planet::update(){
  float currTime = glutGet(GLUT_ELAPSED_TIME)/1000.0;
  float timeDiff = (currTime - lastTime);
 
  lastTime = currTime;

  rotation += rotationVel / (2*M_PI*radius) * (180.0 / M_PI ) * timeDiff * 60;
  Point lastPosition = position;
  if(this->type != SUN){
    // cout<<orbitalVel.getX()<<" "<<orbitalVel.getY()<<" "<<orbitalVel.getZ()<<endl;
    position = Point( position.getX() + timeDiff*orbitalVel.getX(), position.getY() + timeDiff*orbitalVel.getY(), position.getZ() + timeDiff*orbitalVel.getZ() ); 
    // cout<<position.getX()<<" "<<position.getY()<<" "<<position.getZ()<<endl << endl;
  }
  for(unsigned int i=0; i<satellites.size(); i++)
    satellites[i].update();

  planetLine.updatePosition(position/EARTH_RADIUS, lastPosition/EARTH_RADIUS);
  fireball.updatePosition(position/EARTH_RADIUS, lastPosition/EARTH_RADIUS);
  if(lineOn){
    planetLine.update();
  }else{
    planetLine.clearParticles();
  }

  if(onFire){
    fireball.update();
    radius = radius* 0.995;
    if(radius < 1.0) {
      radius = 0;
      onFire = false;
    }
  }else{
    fireball.clearParticles();
  }
  
    
  
  
}
