#include "SkyBox.h"

//Cosntruct the skybox obj
SkyBox::SkyBox()
{
	skyboxShader = Shader("Shaders/SkyBoxVert.txt", "Shaders/SkyBoxFrag.txt");
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*skyBoxVertices.size(), skyBoxVertices.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	textureID =loadCubemap();
	skyboxShader.Activate();
	glUniform1i(glGetUniformLocation(skyboxShader.Shader_ID, "skybox"), 0);
	//skyboxShader.setInt("skybox", 0);
}

//Render the skybox
void SkyBox::Render(glm::mat4 view, glm::mat4 projection)
{
	//glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);  	
	skyboxShader.Activate();

	model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(500, 500, 500));
	skyboxShader.SetMat4("model", model);
	skyboxShader.SetMat4("view", view);
	skyboxShader.SetMat4("projection", projection);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);

}
//Load the cubemap as seperate ppm files
unsigned int SkyBox::loadCubemap()
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		bool textureStatus = dataLoader.LoadTexture(textureData, textureWidth, textureHeight, textureChannels, faces[i]);

		if (textureData.size()!=0)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData.data()
			);
			textureData.clear();
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			textureData.clear();
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	std::cout << "Cubemap tex load Success " << std::endl;
	return textureID;
}

SkyBox::~SkyBox()
{
}
