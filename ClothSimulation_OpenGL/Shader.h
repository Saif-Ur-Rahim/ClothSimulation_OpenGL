#pragma once
#ifndef SHADER_H
#define SHADER_H

#include<GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader
{
public:
	// the compiled shader ID 
	unsigned int Shader_ID;
	Shader();
	// Standard Constructor that takes to shader files and returns you a shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// to Activate the shader
	void Activate();
	// utility  functions
	void SetBool(const std::string &name, bool value) const;
	void SetInt(const std::string &name, int value) const;
	void SetFloat(const std::string &name, float value) const;
	void SetMat4(const std::string &name, glm::mat4 value) const;
	void SetVec3(const std::string & name, float x, float y, float z) const;

private:
	void CheckCompileErrors(unsigned int shader, std::string type);
};
#endif

