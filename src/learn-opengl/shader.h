#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <filesystem>

namespace wmcv
{

struct Shader
{
	Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
	~Shader();

	Shader(Shader&&) = default;
	Shader& operator=(Shader&&) = default;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;

	void on();
	void off();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	unsigned int m_programId;
};

} // namespace wmcv

#endif