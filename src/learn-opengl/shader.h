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

	void setBool(const std::string_view name, bool value) const;
	void setInt(const std::string_view name, int value) const;
	void setFloat(const std::string_view name, float value) const;
	void setMat44(const std::string_view name, const glm::mat4& mat);

	unsigned int m_programId;
};

} // namespace wmcv

#endif