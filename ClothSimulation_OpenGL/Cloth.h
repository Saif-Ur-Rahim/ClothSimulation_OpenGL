#pragma once
#ifndef _GAMEOBJECT_
#define _GAMEOBJECT_
#include<iostream>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include <string>
#include<vector>
#include "Shader.h"
#include "DataLoader.h"
#include "VertexData.h"
#include "FaceData.h"
#include "ObjParticle.h"
#include "ObjSpring.h"
//class ObjSpring;
class Cloth
{
public:
	//Opengl Buffer
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	//Texture properties
	unsigned int texture;
	int textureWidth = 0;
	int textureHeight = 0;
	int textureMaxColumnValue = 0;
	//Mesh Properties
	int meshWidth = 0;
	int meshHeight = 0;
	float scale = 1.0f;
	//Render mode can be quad or tri
	GLenum renderMode;
	//Shader Handle
	Shader shader;
	//Shader matrices
	glm::vec3 worldPosition;
	glm::mat4 translator;
	glm::mat4 model;
	//Loaded Flag
	bool loaded;
	//Data containers for obj file and texture
	std::vector<unsigned int> indices;
	std::vector<unsigned char> textureData;
	std::vector<VertexData> uniqueVertices;
	std::vector<glm::vec2> uniqueTextCoords;
	std::vector<glm::vec3> uniqueNormals;
	std::vector<FaceData> faceDataList;
	std::vector<VertexData> rawVertices;
	//List to store particles
	std::vector<ObjParticle> ObjParticles;
	//List to store springs
	std::vector<ObjSpring> ObjSprings;
	//Edges or daignol points of the cloth
	std::vector<int> EdgePointsIds;
	//Count of connections
	std::vector<int> GeneralConnectionCount;
	//Colors for the particles
	std::vector<glm::vec3> colors{ glm::vec3(1,0,0),glm::vec3(1.0f,0.9f, 0.2f),glm::vec3(0.0f,0.639f,0.101f),glm::vec3(0,0,1),glm::vec3(1,1,1) };

	Cloth();
	//Main Render function
	void Render(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition);
	//Create Object 
	void CreateObject(glm::vec3 worldPosition, float scale, GLenum renderMode, std::string objPath, std::string texturePath);
	//Load data from obj and ppm
	bool LoadModelData(GLenum renderMode, std::string objPath, std::string texturePath);
	//Generate the paticles
	void GenerateParticlesFromObj();
	//Add obj spring to the list
	void AddObjSpring(ObjSpring spring);
	//Destroy the cloth 
	void DestroyObject();

	~Cloth();


};
#endif

