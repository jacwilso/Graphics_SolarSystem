#include <iostream>

#include "solar_system.h"
using namespace std;

Solar_System::Solar_System(){
  glInitNames();
  position = Point(0.0, 0.0, 0.0);
  for(int planInt=SUN; planInt!=COMET; planInt++){
    pick.push_back(false);
    glPushName(planInt+1);
      solar_sys.push_back(new Planet(PLANET(planInt)));
    glPopName();
  }
}

void Solar_System::setShader(GLuint handle, GLuint timeHandle, PLANET planet) {
  for(int planInt = SUN; planInt != COMET; planInt++) {
    if(solar_sys[planInt]->type == planet) {
      solar_sys[planInt]->registerShader(handle, timeHandle);
    }
  }
}

void Solar_System::setTexture(GLuint handle, PLANET planet) {
  for(int planInt = SUN; planInt != COMET; planInt++) {
    if(solar_sys[planInt]->type == planet) {
      solar_sys[planInt]->registerTexture(handle);
    }
  }
}

void Solar_System::specularTexture(GLuint handle, PLANET planet) {
  for(int planInt = SUN; planInt != COMET; planInt++) {
    if(solar_sys[planInt]->type == planet) {
      solar_sys[planInt]->specularTexture(handle);
    }
  }
}

void Solar_System::calculate(){
  xj=0; yj=0; zj=0;
  for(unsigned int i=0; i<solar_sys.size(); i++){
    xDiff[i]=0; yDiff[i]=0; zDiff[0]=0;
    if(i==0) continue;
    float rj = pow( pow(solar_sys[i]->position.getX(), 2) + 
                    pow(solar_sys[i]->position.getY(), 2) + 
                    pow(solar_sys[i]->position.getZ(), 2), 
                    0.5 );
    xj += solar_sys[i]->mass * solar_sys[i]->position.getX() / pow( rj, 3 );
    yj += solar_sys[i]->mass * solar_sys[i]->position.getY() / pow( rj, 3 );
    zj += solar_sys[i]->mass * solar_sys[i]->position.getZ() / pow( rj, 3 );
  }
  
  for(unsigned int i=0; i<solar_sys.size(); i++){
    for(unsigned int j=0; j<solar_sys.size(); j++){
      if(i==j) continue;
      float rj = pow( pow(solar_sys[j]->position.getX() - solar_sys[i]->position.getX(), 2) + 
                      pow(solar_sys[j]->position.getY() - solar_sys[i]->position.getY(), 2) + 
                      pow(solar_sys[j]->position.getZ() - solar_sys[i]->position.getZ(), 2), 
                      0.5 );
      xDiff[i] += G * solar_sys[j]->mass * (solar_sys[j]->position.getX() - solar_sys[i]->position.getX()) / pow( rj, 3 );
      yDiff[i] += G * solar_sys[j]->mass * (solar_sys[j]->position.getY() - solar_sys[i]->position.getY()) / pow( rj, 3 );
      zDiff[i] += G * solar_sys[j]->mass * (solar_sys[j]->position.getZ() - solar_sys[i]->position.getZ()) / pow( rj, 3 );
    }
  }
}

void Solar_System::startTime(){
  for(unsigned int i=0; i<solar_sys.size(); i++)
    solar_sys[i]->startTime();
}

void Solar_System::update(){
  for(unsigned int i=0; i<solar_sys.size(); i++){
    /*
    Vector F = Vector(0,0,0);
    for(unsigned int j=0; j<solar_sys.size(); j++){
      if(i==j) continue;
       F += G * solar_sys[i]->mass * solar_sys[j]->mass * (solar_sys[j]->position - solar_sys[i]->position) / pow((solar_sys[j]->position - solar_sys[i]->position).mag(), 3);
      cout<<"F "<<F.toString()<<endl;
    }
    Vector r = solar_sys[i]->position - Point(0,0,0);
    //solar_sys[i]->orbitalVel = F * r.mag() / solar_sys[i]->mass;*/
    solar_sys[i]->update();
  }
}

void Solar_System::draw(){
  for(unsigned int i=0; i<solar_sys.size(); i++){
    glPushMatrix();
      glTranslatef(solar_sys[i]->position.getX()/EARTH_RADIUS, solar_sys[i]->position.getY()/EARTH_RADIUS, solar_sys[i]->position.getZ()/EARTH_RADIUS); 
      if(pick[i])
        glColor3f(1,0,0);
      else glColor3f(1,1,1);
      solar_sys[i]->draw();
    glPopMatrix();
      //if( solar.pick[i] )
        //cout<<i<<endl;
  }
}

void Solar_System::cleanup(){
  for(unsigned int i=0; i<solar_sys.size(); i++)
    delete solar_sys[i];
}
