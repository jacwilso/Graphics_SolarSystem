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

void Solar_System::setParticleShader(GLuint handle){
  for(int planInt = SUN+1; planInt != COMET; planInt++) {
    solar_sys[planInt]->planetLine.setShader(handle);
    solar_sys[planInt]->fireball.setShader(handle);
  }
}

void Solar_System::easterEgg() {
  for(int planInt = SUN; planInt != COMET; planInt++) {
    solar_sys[planInt]->easterEgg = !solar_sys[planInt]->easterEgg;
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

void Solar_System::setEggTexture(GLuint handle) {
  for(int planInt = SUN; planInt != COMET; planInt++) {
    solar_sys[planInt]->setEggTexture(handle);
  }
}

void Solar_System::update(){
  // for(unsigned int i=0; i<solar_sys.size(); i++){
  //   float ax, ay, az;
  //   for(unsigned int j=0; j<solar_sys.size(); j++){
  //     if(i==j) continue;
  //     ax = -1*G*(solar_sys[i]->mass + solar_sys[j]->mass)*solar_sys[i]->position.getX() / solar_sys[i]->radius - xj + xDiff[i];
  //     ay = -1*G*(solar_sys[i]->mass + solar_sys[j]->mass)*solar_sys[i]->position.getY() / solar_sys[i]->radius - yj + yDiff[i];
  //     az = -1*G*(solar_sys[i]->mass + solar_sys[j]->mass)*solar_sys[i]->position.getZ() / solar_sys[i]->radius - zj + zDiff[i];
  //   }  
  //   float vx = pow( solar_sys[i]->radius*ax , 0.5 );
  //   float vy = pow( solar_sys[i]->radius*ay , 0.5 );
  //   float vz = pow( solar_sys[i]->radius*az , 0.5 );
  //   solar_sys[i]->orbitalVel = Vector( vx, vy, vz );


  //   solar_sys[i]->update();
  // }
  Planet* sun;
  for(unsigned int i=0; i<solar_sys.size(); i++){

    if(solar_sys[i]->type == SUN){
      sun = solar_sys[i];
      break;
    }
  }

  for(unsigned int i=0; i<solar_sys.size(); i++){
    if(i==0)continue;
    Vector orbitalVelDir = cross(solar_sys[i]->position - sun->position,Vector(0,1,0));
    orbitalVelDir.normalize();
    // cout << "Planet " << i << " " << solar_sys[i]->type << endl;
    // cout << G << " " << sun->mass << " " << solar_sys[i]->toSun * pow(10, 6)<< endl;
    // cout<<orbitalVelDir.getX()<<" "<<orbitalVelDir.getY()<<" "<<orbitalVelDir.getZ()<<endl;

    float orbitalVelMag = sqrt((G*sun->mass)/(solar_sys[i]->toSun* pow(10, 6)));
    orbitalVelDir *= orbitalVelMag;
    // cout<<orbitalVelDir.getX()<<" "<<orbitalVelDir.getY()<<" "<<orbitalVelDir.getZ()<<endl;
    // cout << endl;
    solar_sys[i]->orbitalVel = orbitalVelDir;

    solar_sys[i]->update();
  }

  // for(unsigned int i=0; i<solar_sys.size(); i++){
  //   if(i==0)continue;
  //   Vector gravityDir = sun->position - solar_sys[i]->position;
  //   gravityDir.normalize();
  //   cout << "Planet " << i << " " << solar_sys[i]->type << endl;
  //   cout << G << " " << sun->mass << " " << solar_sys[i]->toSun * pow(10, 6)<< endl;
  //   cout<<gravityDir.getX()<<" "<<gravityDir.getY()<<" "<<gravityDir.getZ()<<endl;

  //   double gravityMag = G*((double)sun->mass)/pow((double)solar_sys[i]->toSun* pow(10, 6),2);
  //   cout << gravityMag << endl;
  //   gravityDir *= gravityMag;
  //   cout<<gravityDir.getX()<<" "<<gravityDir.getY()<<" "<<gravityDir.getZ()<<endl;
  //   //cout << endl;
  //   solar_sys[i]->orbitalVel += gravityDir * 1000.0;

  //   solar_sys[i]->update();
  // }

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
