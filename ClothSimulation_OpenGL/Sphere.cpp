#include "Sphere.h"


//Constructor 
Sphere::Sphere()
{
}
//Main Render fucntion
void Sphere::Render(glm::mat4 view, glm::mat4 projection, glm::vec3 cameraPosition,bool rotate)
{
	//Enable Texturing
	glEnable(GL_TEXTURE_2D);
	shader.Activate();
	//Set up matrices and light vector and pass them to the shader
	translator = glm::mat4(1.0f);
	if (rotate)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, worldPosition);
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	shader.SetVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader.SetVec3("lightPos", 1.2f, 1.0f, 2.0f);
	shader.SetVec3("viewPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);
	shader.SetMat4("transform", translator);
	shader.SetMat4("view", view);
	shader.SetMat4("projection", projection);
	shader.SetMat4("model", model);

	//Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	//Bind VAO
	glBindVertexArray(VAO);
	//Draw 
	glDrawElements(renderMode, indices.size(), GL_UNSIGNED_INT, 0);
}

//Create the object sphere
void Sphere::CreateObject(glm::vec3 worldPosition, float scale, GLenum renderMode, std::string objPath, std::string texturePath)
{
	//Load OBJ
	bool loaded = LoadModelData(renderMode, objPath, texturePath);
	if (!loaded)
	{
		exit(0);
	}
	//Setup world position and matrices for shader
	this->worldPosition = worldPosition;
	this->scale = scale;
	translator = glm::mat4(1.0f);

	model = glm::mat4(1.0f);
	model = glm::translate(model, worldPosition);
	this->renderMode = renderMode;
	//add the texture coordinates
	for (int i = 0; i < uniqueVertices.size(); i++)
	{
		uniqueVertices[i].texCoord[0] = uniqueTextCoords[i].x;
		uniqueVertices[i].texCoord[1] = uniqueTextCoords[i].y;
	}
	//load the shader
	shader = Shader("Shaders/GameObjectBasicVertex.txt", "Shaders/GameObjectBasicFragment.txt");
	
	//Bind vertex buffer and index buffer
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) *uniqueVertices.size(), uniqueVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) *indices.size(), indices.data(), GL_STATIC_DRAW);

	//Setup the layout binding
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	//Setup the texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data());

	glGenerateMipmap(GL_TEXTURE_2D);

	glUniform1i(glGetUniformLocation(shader.Shader_ID, "textureData"), 0);
	//Setup the radius of the sphere
	glm::vec3 radiusVector;
	radiusVector.x = worldPosition.x - uniqueVertices[0].position[0];
	radiusVector.y = worldPosition.y - uniqueVertices[0].position[1];
	radiusVector.z = worldPosition.z - uniqueVertices[0].position[2];
	Radius = glm::length(radiusVector);
	std::cout << Radius << std::endl;




}
//Load the obj file and texture 
bool Sphere::LoadModelData(GLenum renderMode, std::string objPath, std::string texturePath)
{
	DataLoader loader = DataLoader();
	bool objStatus = false;
	bool textureStatus = false;

	objStatus = loader.ReadObjFileTriFace(uniqueVertices, uniqueNormals, uniqueTextCoords, indices, objPath);
	textureStatus = loader.LoadTexture(textureData, textureWidth, textureHeight, textureMaxColumnValue, texturePath);

	return textureStatus && objStatus;
}

Sphere::~Sphere()
{
}
