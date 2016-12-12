#include "ParticleSystem.h"

Vector viewVector;
Point cameraXZ;

// Sort your particles by distance, so you draw them back->front and avoid transparency shenanigans
struct particle_sort{
    inline bool operator() (Particle* particle1, Particle* particle2){
        
        return (dot((particle1->getLocation() - cameraXZ), viewVector) > dot((particle2->getLocation() - cameraXZ), viewVector));
    }
};

void ParticleSystem::calculateNetForce(){
	Vector sum = Vector(0.0,0.0,0.0);
	for (unsigned int i = 0; i < forces.size(); i++){
		sum = sum + forces[i];
	}

	netForce = sum;
}

void ParticleSystem::setShader(GLuint newProgramHandle){
	this->shaderProgramHandle = newProgramHandle;
	uniformAgeLoc = glGetUniformLocation(shaderProgramHandle, "age");
	uniformLifeLoc = glGetUniformLocation(shaderProgramHandle, "lifespan");
}


void ParticleSystem::update(){
		
	//Spawn new particles at the specified rate
	spawnCounter += spawnRate;
	if(spawnCounter >= 1.0){
		spawnParticle();
		spawnCounter = spawnCounter - 1.0;
	}

	// Update the particles, and check if they bounce/die off
	for (unsigned int i = 0; i < particles.size(); i++){
		Particle* particle = particles[i];
		Point currLocation = particle->getLocation();
		Vector velocity = particle->getVelocity();
		double age = particle->getAge();

		currLocation = currLocation + velocity;

		calculateNetForce();
		velocity = velocity + netForce;

		//Age the particle
		age = age + TIME_CONSTANT;

		//Bounce off the ground
		if(currLocation.getY() <= (-1.0 * location.getY()) ){
			velocity.setY(velocity.getY() * -0.3);
			currLocation.setY(0);
		}

		//update the particle
		particle->setLocation(currLocation);
		particle->setVelocity(velocity);
		particle->setAge(age);

		//Kill of the particle when it lives out its life
		if(age >= particle->getLifespan()){
			particles[i] = particles.back();
			particles.pop_back();
			delete particle;
		}
	}
}


void ParticleSystem::draw(int particleType){

	// specify each of our material component colors
	GLfloat diffCol[4] = { 1.0, 1.0, 1.0 };
	GLfloat specCol[4] = { 0.2, 0.2, 0.2 };
	GLfloat ambCol[4] = { 1.0, 1.0, 1.0 };

	// and now set them for the front and back faces
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, diffCol );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, specCol );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 0.0 ); 
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, ambCol );

	glPushMatrix();
		//glTranslatef(location.getX(), location.getY(), location.getZ());

		// Set up our particle shader
		glUseProgram(shaderProgramHandle);	    

		// Enable texturing
		glEnable(GL_TEXTURE_2D);

        //Set our texture
		for (unsigned int i = 0; i < particles.size(); i++){

			//Pass the particle's age information to the shader
			glUniform1f(uniformAgeLoc, particles[i]->getAge());
			glUniform1f(uniformLifeLoc, particles[i]->getLifespan());
			
			// Draw the particle
			particles[i]->draw();
		}

		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void ParticleSystem::clearParticles(){
	for (unsigned int i; i < particles.size(); i++)
   {
     delete(particles[i]);
   } 
   particles.clear();
}

void ParticleSystem::printStats(){
	printf("Particles in System: %i\n", particles.size());
	printf("Spawn Counter: %2.2f\n", spawnCounter);
	printf("netForce: %2.2f\n", netForce.mag());
}