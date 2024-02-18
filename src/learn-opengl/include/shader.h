#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <filesystem>

namespace wmcv
{

struct Shader
{
	Shader() = default;
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
	void setVec2(const std::string_view name, glm::vec2 value) const;
	void setVec3(const std::string_view name, glm::vec3 value) const;
	void setVec4(const std::string_view name, glm::vec4 value) const;
	void setMat2(const std::string_view name, const glm::mat2& mat);
	void setMat3(const std::string_view name, const glm::mat3& mat);
	void setMat4(const std::string_view name, const glm::mat4& mat);

	unsigned int m_programId;
};

} // namespace wmcv

#endif