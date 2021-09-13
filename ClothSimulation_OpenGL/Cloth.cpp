#include "Cloth.h"


Cloth::Cloth()
{
	
}

void Cloth::CreateObject(glm::vec3 worldPosition,float scale, GLenum renderMode, std::string objPath, std::string texturePath)
{
	//Load the obj and ppm
	loaded = LoadModelData(renderMode, objPath, texturePath);
	if (!loaded)
	{
		exit(0);
	}
	//Generate the particles
	GenerateParticlesFromObj();
	//Setup the vectors and matrices for the shader
	this->worldPosition = worldPosition;
	this->scale = scale;
	translator = glm::mat4(1.0f);

	model = glm::mat4(1.0f);
	model = glm::translate(model, worldPosition);
	this->renderMode = renderMode;

	//Setup the texture coordinates
	for (int i = 0; i < uniqueVertices.size(); i++)
	{
	  uniqueVertices[i].texCoord[0] = uniqueTextCoords[i].x;
	  uniqueVertices[i].texCoord[1] = uniqueTextCoords[i].y;
	}
	//Load the shader
	shader = Shader("Shaders/ClothShaderVertex.txt", "Shaders/ClothShaderFrag.txt");
	//Bind Vertex and index buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) *uniqueVertices.size(), uniqueVertices.data(), GL_STATIC_DRAW);

	//Setup the layout binding
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);


	//Bind the index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) *indices.size(), indices.data(), GL_STATIC_DRAW);

	//Setup and bind the textures
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,textureData.data());

	glGenerateMipmap(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(shader.Shader_ID, "textureData"), 0);

}
//Main function to render the cloth
void Cloth::Render(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition)
{
	if (loaded)
	{
		//Enable the texturing
		glEnable(GL_TEXTURE_2D);
		shader.Activate();
		//Send the matrices and vectors to the shader
		translator = glm::mat4(1.0f);

		shader.SetMat4("transform", translator);
		shader.SetMat4("view", view);
		shader.SetMat4("projection", projection);
		shader.SetMat4("model", model);
		shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
		shader.SetVec3("lightPos", 1.2f, 1.0f, 2.0f);
	
		shader.SetVec3("viewPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);
		//Bind the texture 
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		//bind VAO
		glBindVertexArray(VAO);
		//Bind the vertices continously as we are updating them continously
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData)*uniqueVertices.size(), uniqueVertices.data(), GL_STATIC_DRAW);

		glDrawElements(renderMode, indices.size(), GL_UNSIGNED_INT, 0);
	}
	
}
//Load the OBJ file and ppm texture
bool Cloth::LoadModelData(GLenum renderMode,std::string objPath, std::string texturePath)
{
	DataLoader loader = DataLoader();
	bool objStatus = false;
	bool textureStatus = false;

	objStatus = loader.ReadObjFileQuadFace(uniqueVertices,uniqueNormals,uniqueTextCoords,faceDataList, objPath);

	textureStatus = loader.LoadTexture(textureData, textureWidth, textureHeight, textureMaxColumnValue, texturePath);


	return textureStatus && objStatus;

}

//Generate the particles
void Cloth::GenerateParticlesFromObj()
{
	//Use unique vertices to get particles
	for (int i = 0; i < uniqueVertices.size(); i++)
	{
		ObjParticle particle = ObjParticle(i,*this);
		ObjParticles.push_back(particle);
	}

	//Loop the faces
	for (int i = 0; i < faceDataList.size(); i++)
	{
		//Push the indices
		indices.push_back(faceDataList[i].vertexIndex[0]);
		indices.push_back(faceDataList[i].vertexIndex[1]);
		indices.push_back(faceDataList[i].vertexIndex[2]);

		indices.push_back(faceDataList[i].vertexIndex[0]);
		indices.push_back(faceDataList[i].vertexIndex[2]);
		indices.push_back(faceDataList[i].vertexIndex[3]);
		
		//Create springs for a quad face
		ObjSpring spring1 = ObjSpring(faceDataList[i].vertexIndex[0], faceDataList[i].vertexIndex[1], *this);
		ObjSpring spring2 = ObjSpring(faceDataList[i].vertexIndex[0], faceDataList[i].vertexIndex[3], *this);
		ObjSpring spring3 = ObjSpring(faceDataList[i].vertexIndex[0], faceDataList[i].vertexIndex[2], *this);

		ObjSpring spring4 = ObjSpring(faceDataList[i].vertexIndex[1], faceDataList[i].vertexIndex[2], *this);
		ObjSpring spring5 = ObjSpring(faceDataList[i].vertexIndex[1], faceDataList[i].vertexIndex[3], *this);
		ObjSpring spring6 = ObjSpring(faceDataList[i].vertexIndex[3], faceDataList[i].vertexIndex[2], *this);
		
		//Add the springs
		AddObjSpring(spring1);
		AddObjSpring(spring2);
		AddObjSpring(spring3);
		AddObjSpring(spring4);
		AddObjSpring(spring5);
		AddObjSpring(spring6);

	}
	//Following loop get the Edge point or diagnol points of the square cloth
	int tempConnection = 0;
	for (int i = 0; i < ObjParticles.size(); i++)
	{
		if (tempConnection != ObjParticles[i].connections.size())
		{
			tempConnection = ObjParticles[i].connections.size();
			GeneralConnectionCount.push_back(tempConnection);
		}
		if (ObjParticles[i].connections.size() == 3)
		{
		
			EdgePointsIds.push_back(ObjParticles[i].Id);

			std::cout << "Index : " << i << " X : " <<ObjParticles[i].em_position.x << "Y : " << ObjParticles[i].em_position.y << "Z : " << ObjParticles[i].em_position.z << std::endl;
		}
	}

	//Setup the color of the paticle and match them with vertices
	for (int i = 0; i < ObjParticles.size(); i++)
	{
		std::vector<int>::iterator it1 = std::find(GeneralConnectionCount.begin(), GeneralConnectionCount.end(), ObjParticles[i].connections.size());
		if (it1 != GeneralConnectionCount.end())
		{
			int index = std::distance(GeneralConnectionCount.begin(), it1);
			if (index < colors.size())
			{
				uniqueVertices[ObjParticles[i].Id].color[0] = colors[index].x;
				uniqueVertices[ObjParticles[i].Id].color[1] = colors[index].y;
				uniqueVertices[ObjParticles[i].Id].color[2] = colors[index].z;
			}
			else
			{
				uniqueVertices[ObjParticles[i].Id].color[0] = colors[colors.size() - 1].x;
				uniqueVertices[ObjParticles[i].Id].color[1] = colors[colors.size() - 1].y;
				uniqueVertices[ObjParticles[i].Id].color[2] = colors[colors.size() - 1].z;
			}

		}
		else
		{
			uniqueVertices[ObjParticles[i].Id].color[0] = colors[colors.size() - 1].x;
			uniqueVertices[ObjParticles[i].Id].color[1] = colors[colors.size() - 1].y;
			uniqueVertices[ObjParticles[i].Id].color[2] = colors[colors.size() - 1].z;	
		}
	}
	//This code generate the 4 bending spring. I have commented as i was not satisfied with them you can uncomment and check them
	/*for (int i = 0; i < EdgePointsIds.size(); i++)
	{
		for (int j = 0; j < EdgePointsIds.size(); j++)
		{
			if (j != i)
			{
				ObjSpring spring = ObjSpring(EdgePointsIds[i], EdgePointsIds[j], *this);
			    AddObjSpring(spring);
			}
		}
	}*/
}

void Cloth::DestroyObject()
{
}

//Add a spring if it is not present
void Cloth::AddObjSpring(ObjSpring spring)
{
	std::vector<ObjSpring>::iterator it1 = std::find(ObjSprings.begin(), ObjSprings.end(), spring);
	if (it1 != ObjSprings.end())
	{
	
	}
	else
	{
		ObjParticles[spring.endPointId_1].connections.push_back(spring.endPointId_2);
		ObjParticles[spring.endPointId_2].connections.push_back(spring.endPointId_1);
		ObjSprings.push_back(spring);
	}
}




//Free all the memory taken by cloth
Cloth::~Cloth()
{
	std::cout << "Cloth Destroyed" << std::endl;
	loaded = false;
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	uniqueVertices.clear();
	uniqueNormals.clear();
	uniqueTextCoords.clear();
	EdgePointsIds.clear();
	indices.clear();
	textureData.clear();
	faceDataList.clear();
	rawVertices.clear();
	ObjParticles.clear();
	ObjSprings.clear();
	EdgePointsIds.clear();
	GeneralConnectionCount.clear();
	
}
