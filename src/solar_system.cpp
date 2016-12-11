#include <iostream>

#include "solar_system.h"
using namespace std;

Solar_System::Solar_System(){
  position = Point(0.0, 0.0, 0.0);
  for(int planInt=SUN; planInt!=COMET; planInt++){
    pick.push_back(false);
    solar_sys.push_back(new Planet(PLANET(planInt)));
  }
}

void Solar_System::setShader(GLuint handle, GLuint timeHandle, PLANET planet) {
  for(int planInt = SUN; planInt != COMET; planInt++) {
    if(solar_sys[planInt]->type == planet) {
      solar_sys[planInt]->registerShader(handle, timeHandle);
    }
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
    /*
    cout << "Planet " << i << " " << solar_sys[i]->type << endl;
    cout << G << " " << sun->mass << " " << solar_sys[i]->toSun * pow(10, 6)<< endl;
    cout<<orbitalVelDir.getX()<<" "<<orbitalVelDir.getY()<<" "<<orbitalVelDir.getZ()<<endl;
    */
    float orbitalVelMag = sqrt((G*sun->mass)/(solar_sys[i]->toSun* pow(10, 6)));
    orbitalVelDir *= orbitalVelMag;
    /*
    cout<<orbitalVelDir.getX()<<" "<<orbitalVelDir.getY()<<" "<<orbitalVelDir.getZ()<<endl;
    cout << endl;
    */
    solar_sys[i]->orbitalVel = orbitalVelDir;

    solar_sys[i]->update();
  }
}

void Solar_System::draw(){
  for(unsigned int i=0; i<solar_sys.size(); i++){
    glPushMatrix();
      glTranslatef(solar_sys[i]->position.getX()/EARTH_RADIUS, solar_sys[i]->position.getY()/EARTH_RADIUS, solar_sys[i]->position.getZ()/EARTH_RADIUS); 
      if( pick[i] )
        glutSolidSphere(solar_sys[i]->radius/EARTH_RADIUS,32,32);
      solar_sys[i]->draw();
    glPopMatrix();
    if( pick[i] )
      cout<<"PICK "<<i<<endl;
  }
}

void Solar_System::cleanup(){
  for(unsigned int i=0; i<solar_sys.size(); i++)
    delete solar_sys[i];
}

void Solar_System::newPlanet(Point position){
  //solar_sys.push_back(new Planet(position, EARTH_RADIUS/2.0, EARTH_MASS/3.0, ));
  //solar_sys[solar_sys.size()-1].startTime();
}
