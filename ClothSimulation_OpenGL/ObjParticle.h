#pragma once
#ifndef _OBJPARTICLE_
#define _OBJPARTICLE_
#include<iostream>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include <string>
#include<vector>
#include"VertexData.h";
class Spring;
class SpringPoll;
class Cloth;
//Main class for particles
class ObjParticle
{
public:
	//Id of particle
	int Id;
	//mass of particle em_ shows its a part of physics system operating on euler
	float em_mass = 5.0f;
	//pointer to main cloth
	Cloth *cloth;
	//Static state flag, em_ shows its a part of physics system operating on euler
	bool em_Static = false;
    //Accelration,em_ shows its a part of physics system operating on euler
	glm::vec3 em_accelration;
	//Rest Position,em_ shows its a part of physics system operating on euler
	glm::vec3 em_restPosition;
	//velocity,em_ shows its a part of physics system operating on euler
	glm::vec3 em_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	//Total force,em_ shows its a part of physics system operating on euler
	glm::vec3 em_force_sum = glm::vec3(0.0f, 0.0f, 0.0f);
	//Previous frame position,em_ shows its a part of physics system operating on euler (i did verlet as well, but due to unstablity removed it, this member was extensively used in that)
	glm::vec3 em_previousPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	//A handle to store temp position,em_ shows its a part of physics system operating on euler
	glm::vec3 em_position = glm::vec3(0.0f, 0.0f, 0.0f);
	//Connected to which particle
	std::vector<int> connections;
	//Constructor
	ObjParticle(int Id, Cloth & g);
	//Remove all force,em_ shows its a part of physics system operating on euler
	void em_removeForceSum();
	//Apply Force,em_ shows its a part of physics system operating on euler
	void em_applyForce(glm::vec3 force);
	//Calculate the new position,em_ shows its a part of physics system operating on euler
	void em_calculatePosition(float deltaTime);
	//Main Physics loop, em_ shows its a part of physics system operating on euler
	void em_runPhysicsLoop(float deltaTime);
	//Check collison with sphere
	void CheckCollision(glm::vec3 sphereWorldPosition,float sphereRadius, bool collisionStatic,bool rotate);

	ObjParticle();
	~ObjParticle();
};
#endif
