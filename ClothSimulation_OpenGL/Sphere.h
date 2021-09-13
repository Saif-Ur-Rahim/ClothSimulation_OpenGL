#pragma once
#ifndef _SPHERE_
#define _SPHERE_
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
#include<GLFW/glfw3.h>
//Main class to represent the sphere
class Sphere
{
public:
	//Buffers for opengl
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	//Texture Properties
	unsigned int texture;
	int textureWidth = 0;
	int textureHeight = 0;
	int textureMaxColumnValue = 0;
	//Mesh properties
	int meshWidth = 0;
	int meshHeight = 0;
	float scale = 1.0f;
	//Radius
	float Radius = 0.0f;
	//Render mode can be quad or tri
	GLenum renderMode;
	//Shader
	Shader shader;
	//WorldPosition
	glm::vec3 worldPosition;
	//Translator
	glm::mat4 translator;
	//Model
	glm::mat4 model;
	//OBJ File data containers
	std::vector<unsigned int> indices;
	std::vector<unsigned char> textureData;
	std::vector<VertexData> uniqueVertices;
	std::vector<glm::vec2> uniqueTextCoords;
	std::vector<glm::vec3> uniqueNormals;
	
	//Constructor
	Sphere();
	//Main function to render the object
	void Render(glm::mat4 view, glm::mat4 projection,glm::vec3 cameraPosition,bool rotate);
	//Create the object 
	void CreateObject(glm::vec3 worldPosition, float scale, GLenum renderMode, std::string objPath, std::string texturePath);
	//Load the OBJ File that has the sphere
	bool LoadModelData(GLenum renderMode, std::string objPath, std::string texturePath);
	~Sphere();
};
#endif
