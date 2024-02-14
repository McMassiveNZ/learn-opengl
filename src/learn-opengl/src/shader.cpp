#include "pch.h"
#include "shader.h"
#include "opengl.h"

#include "wmcv_log/wmcv_log.h"

#include <fstream>
#include <sstream>

namespace wmcv
{

Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		wmcv::LogMessage("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		wmcv::LogMessage("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", infoLog); 
	};

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		wmcv::LogMessage("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", infoLog); 
	};

	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertex);
	glAttachShader(m_programId, fragment);
	glLinkProgram(m_programId);

	glGetProgramiv(m_programId, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_programId, 512, NULL, infoLog);
		wmcv::LogMessage("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}", infoLog);
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteShader(m_programId);
}

void Shader::on()
{
	glUseProgram(m_programId);
}

void Shader::off()
{
	glUseProgram(0);
}

void Shader::setBool(const std::string_view name, bool value) const
{
	const GLint location = glGetUniformLocation(m_programId, name.data());
	glUniform1i(location, static_cast<int>(value));
}

void Shader::setInt(const std::string_view name, int value) const
{
	const GLint location = glGetUniformLocation(m_programId, name.data());
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string_view name, float value) const
{
	const GLint location = glGetUniformLocation(m_programId, name.data());
	glUniform1f(location, value);
}

void Shader::setMat44(const std::string_view name, const glm::mat4& value)
{
	const GLint location = glGetUniformLocation(m_programId, name.data());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

}