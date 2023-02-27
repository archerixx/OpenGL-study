#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vCode = readFromFile(vertexPath);
	const char* vertexCode = vCode.c_str();
	std::cout << vertexCode << std::endl;

	std::string fCode = readFromFile(fragmentPath);
	const char* fragmentCode = fCode.c_str();
	std::cout << fragmentCode << std::endl;

	unsigned int vertex, fragment;
	compileShader(vertex, vertexCode, GL_VERTEX_SHADER, "VERTEX");
	compileShader(fragment, fragmentCode, GL_FRAGMENT_SHADER, "FRAGMENT");

	int success;
	char infoLog[512];
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
			infoLog << std::endl;
	}
	// delete shaders; they’re linked into our program and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

std::string Shader::readFromFile(const char* shaderPath) 
{
	std::ifstream shaderFile;
	std::stringstream shaderStream;

	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	// open file, read from buffer into stream, close file and read streams content into code
	try {

		shaderFile.open(shaderPath);
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return shaderStream.str();
}

void Shader::compileShader(unsigned int &shader, const char* shaderCode, GLenum shaderType, const char* type)
{
	int success;
	char infoLog[512];
	// vertex Shader
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	// print compile errors if any
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::dispose()
{
	glDeleteProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}