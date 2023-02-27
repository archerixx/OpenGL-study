#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// the program ID
	unsigned int ID;
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	// use/activate the shader
	void use();
	void dispose();
	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

private:
	// read from file
	std::string readFromFile(const char* shaderPath);
	void compileShader(unsigned int& shader, const char* shaderCode, GLenum shaderType, const char* type);
};