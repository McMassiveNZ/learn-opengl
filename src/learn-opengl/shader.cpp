#include "pch.h"
#include "shader.h"
#include "opengl.h"

#include <fstream>
#include <sstream>
#include <iostream>

namespace wmcv
{

Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	};

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	};

	// shader Program
	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertex);
	glAttachShader(m_programId, fragment);
	glLinkProgram(m_programId);
	// print linking errors if any
	glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_programId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
				  << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteShader(m_programId);
}

void Shader::use()
{
	glUseProgram(m_programId);
}

void Shader::setBool(const std::string& name, bool value) const
{
	const GLint location = glGetUniformLocation(m_programId, name.c_str());
	glUniform1i(location, static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const
{
	const GLint location = glGetUniformLocation(m_programId, name.c_str());
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	const GLint location = glGetUniformLocation(m_programId, name.c_str());
	glUniform1f(location, value);
}

}