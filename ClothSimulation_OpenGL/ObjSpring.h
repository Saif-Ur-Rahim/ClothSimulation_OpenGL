#pragma once
#ifndef _OBJSPRING_
#define _OBJSPRING_
#include<iostream>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include <string>
#include<vector>
#include"Shader.h"
#include"VertexData.h"
class Cloth;
//Main Spring Class
class ObjSpring
{
public:
	//End points or vertices of spring
	float endPointId_1;
	float endPointId_2;
	//Rest lenght
	float springRestLenght;
	//Pointer to main cloth
	Cloth *cloth;
	//Rest Positions
	glm::vec3 pointFirstRestPosition;
	glm::vec3 pointSecondRestPosition;
	//Constructor
	ObjSpring(int id1, int id2, Cloth &g);
	//Function to run the spring force system. em_ show that it is part of physics engine
	void em_runSpringForceSystem(float ks, float kd, bool reverse);
	//Custom operator for comparison to remove duplicate spring and speed things up
	bool operator==(const ObjSpring& other) const
	{
		if (endPointId_1 == other.endPointId_1 && endPointId_2 == other.endPointId_2)
		{
			return true;
		}
		else if (endPointId_1 == other.endPointId_2 && endPointId_2 == other.endPointId_1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	ObjSpring();
	~ObjSpring();
};
#endif
