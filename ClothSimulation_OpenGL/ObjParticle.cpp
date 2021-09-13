#include "ObjParticle.h"
#include "Cloth.h"

//Constructor
ObjParticle::ObjParticle(int Id, Cloth& g)
{
	//Setup the members
	this->Id = Id;
	cloth = &g;
	em_restPosition.x = cloth->uniqueVertices[Id].position[0];
	em_restPosition.y = cloth->uniqueVertices[Id].position[1];
	em_restPosition.z = cloth->uniqueVertices[Id].position[2];

	em_previousPosition.x = cloth->uniqueVertices[Id].position[0];
	em_previousPosition.y = cloth->uniqueVertices[Id].position[1];
	em_previousPosition.z = cloth->uniqueVertices[Id].position[2];

	em_position.x = cloth->uniqueVertices[Id].position[0];
	em_position.y = cloth->uniqueVertices[Id].position[1];
	em_position.z = cloth->uniqueVertices[Id].position[2];
}
//Function removes all the forces and  velocity,em_ shows its a part of physics system operating on euler
void ObjParticle::em_removeForceSum()
{
	em_velocity  = glm::vec3(0.0f, 0.0f, 0.0f);

	em_force_sum = glm::vec3(0.0f, 0.0f, 0.0f);
}
//Function add force to the total force,em_ shows its a part of physics system operating on euler
void ObjParticle::em_applyForce(glm::vec3 force)
{
	em_force_sum += force;
}
//Calculate the new positions of particle,em_ shows its a part of physics system operating on euler
void ObjParticle::em_calculatePosition(float deltaTime)
{
	//initial pos and vel
	glm::vec3 initialPoisiton = em_position;
	glm::vec3 initialVelocity = em_velocity;
	
	//a = F/m
	em_accelration.x = em_force_sum.x / em_mass;
	em_accelration.y = em_force_sum.y / em_mass;
	em_accelration.z = em_force_sum.z / em_mass;

	//final vel = vi  + delta time * accel
	glm::vec3  finalVelocity = initialVelocity +  (deltaTime *em_accelration);
	//final pos = posI + deltatime * finalvel
	glm::vec3  finalPosition = initialPoisiton +  (deltaTime *finalVelocity);

	//Set the new data
	em_position = finalPosition;
	em_velocity = finalVelocity;
	
}

//Main physics loop, em_ shows its a part of physics system operating on euler
void ObjParticle::em_runPhysicsLoop(float deltaTime)
{
	em_calculatePosition(deltaTime);
	
	if (!em_Static)
	{	
		cloth->uniqueVertices[Id].position[0] = em_position.x;
		cloth->uniqueVertices[Id].position[1] = em_position.y;
		cloth->uniqueVertices[Id].position[2] = em_position.z;
	}
}

//Calculate the collision with spehre
void ObjParticle::CheckCollision(glm::vec3 sphereWorldPosition, float sphereRadius,bool collisionStatic,bool rotate)
{
	//set a radius of particle
	float particleRadius = 0.2f;
	//Get the direction vector
	glm::vec3 particleToSphere = em_position - sphereWorldPosition;
	//Get the distance from point of collision to particle
	float collisionDistance = glm::length(particleToSphere) - (sphereRadius + particleRadius);
	//Normalize vector
	glm::vec3 particleToSphereNormal = glm::normalize(particleToSphere);
	//Calculate the new vector to move the particle
	glm::vec3 moveVector = particleToSphereNormal * collisionDistance;

	if (collisionDistance < 0)
	{
		if (collisionStatic)
		{
			em_removeForceSum();
			em_Static = true;
		}
		//Add a new velocity 
		em_velocity = em_velocity * 0.8f;

		//Move in opposite direction
		em_position -= (moveVector *1.1f);
		if (rotate)
		{
			em_position -= em_velocity * -1.0f *0.4f;
		}

		cloth->uniqueVertices[Id].position[0] = em_position.x;
		cloth->uniqueVertices[Id].position[1] = em_position.y;
		cloth->uniqueVertices[Id].position[2] = em_position.z;
		
	}

}

ObjParticle::ObjParticle()
{
}


ObjParticle::~ObjParticle()
{
}
