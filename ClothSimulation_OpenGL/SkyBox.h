#pragma once
#ifndef _SKYBOX_
#define _SKYBOX_
#include<iostream>
#include<glm.hpp>
#include<gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include <string>
#include<vector>
#include "Shader.h"
#include "DataLoader.h"
class SkyBox
{
public:
	std::vector<std::string> faces
	{
		"SkyBox/rightImage.ppm",
		"SkyBox/leftImage.ppm",
		"SkyBox/topImage.ppm",
		"SkyBox/bottomImage.ppm",
		"SkyBox/frontImage.ppm",
		"SkyBox/backImage.ppm"
	};
	std::vector<float> skyBoxVertices {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	unsigned int skyboxVAO, skyboxVBO;
	DataLoader dataLoader;
	unsigned int textureID;
	int textureWidth, textureHeight, textureChannels;
	std::vector<unsigned char> textureData;
	Shader skyboxShader;
	glm::mat4 model = glm::mat4(1.0f);
	SkyBox();

	void Render(glm::mat4 view, glm::mat4 projection);
	unsigned int loadCubemap();
	~SkyBox();
};
#endif
