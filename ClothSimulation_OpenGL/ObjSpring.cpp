#include "ObjSpring.h"
#include "Cloth.h"


//Constructor
ObjSpring::ObjSpring(int id1, int id2, Cloth & g)
{
	//Setup members
	endPointId_1 = id1;
	endPointId_2 = id2;
	cloth = &g;
	pointFirstRestPosition = glm::vec3(cloth->uniqueVertices[endPointId_1].position[0], cloth->uniqueVertices[endPointId_1].position[1], cloth->uniqueVertices[endPointId_1].position[2]);
	pointSecondRestPosition = glm::vec3(cloth->uniqueVertices[endPointId_2].position[0], cloth->uniqueVertices[endPointId_2].position[1], cloth->uniqueVertices[endPointId_2].position[2]);
	//Calculate rest lenght
	springRestLenght = glm::length(pointFirstRestPosition - pointSecondRestPosition);

}
//Main Physics system for spring it uses euler method
void ObjSpring::em_runSpringForceSystem(float ks, float kd, bool reverse)
{
	//Delta P of the equation
	glm::vec3 deltaPEndPoint;
	deltaPEndPoint.x = cloth->uniqueVertices[endPointId_2].position[0] - cloth->uniqueVertices[endPointId_1].position[0];
	deltaPEndPoint.y = cloth->uniqueVertices[endPointId_2].position[1] - cloth->uniqueVertices[endPointId_1].position[1];
	deltaPEndPoint.z = cloth->uniqueVertices[endPointId_2].position[2] - cloth->uniqueVertices[endPointId_1].position[2];
	//Get Delta P lenght
	float leghtDeltaPEP = glm::length(deltaPEndPoint);
	//Normalize the Delta P
	deltaPEndPoint.x = deltaPEndPoint.x / leghtDeltaPEP;
	deltaPEndPoint.y = deltaPEndPoint.y / leghtDeltaPEP;
	deltaPEndPoint.z = deltaPEndPoint.z / leghtDeltaPEP;
	//Get the particles attached to the spring
	ObjParticle particle1 = cloth->ObjParticles[endPointId_1];
	ObjParticle particle2 = cloth->ObjParticles[endPointId_2];
	//Dot product of v and delta p
	float v1 = deltaPEndPoint.x * particle1.em_velocity.x + deltaPEndPoint.y * particle1.em_velocity.y + deltaPEndPoint.z * particle1.em_velocity.z;
	float v2 = deltaPEndPoint.x * particle2.em_velocity.x + deltaPEndPoint.y * particle2.em_velocity.y + deltaPEndPoint.z * particle2.em_velocity.z;
	//Calculate for current frame lenght
	glm::vec3 currentLegnth;
	currentLegnth.x = cloth->uniqueVertices[endPointId_2].position[0] - cloth->uniqueVertices[endPointId_1].position[0];
	currentLegnth.y = cloth->uniqueVertices[endPointId_2].position[1] - cloth->uniqueVertices[endPointId_1].position[1];
	currentLegnth.z = cloth->uniqueVertices[endPointId_2].position[2] - cloth->uniqueVertices[endPointId_1].position[2];
	//Calculate the spring force
	float springForce = -ks * (springRestLenght - glm::length(currentLegnth)) - kd * (v1 - v2);
	//Finalize the force
	float forcex = springForce * deltaPEndPoint.x;
	float forcey = springForce * deltaPEndPoint.y;
	float forcez = springForce * deltaPEndPoint.z;
	//Apply to particles
	cloth->ObjParticles[endPointId_1].em_applyForce(glm::vec3(forcex, forcey, forcez));
	cloth->ObjParticles[endPointId_2].em_applyForce(glm::vec3(-1.0f*forcex, -1.0f*forcey, -1.0f*forcez));


}

ObjSpring::ObjSpring()
{
}

ObjSpring::~ObjSpring()
{
}
