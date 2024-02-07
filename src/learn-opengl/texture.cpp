#include "pch.h"
#include "texture.h"
#include "opengl.h"

#include "wmcv_log/wmcv_log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace wmcv
{

Texture::Texture(const std::filesystem::path& path, bool flip)
{
	const auto alpha = path.extension() == std::filesystem::path(".png");

	stbi_set_flip_vertically_on_load(static_cast<int>(flip));

	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int32_t width, height, nrChannels;
	if (unsigned char* raw = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0); raw)
	{
		//GL_RGB is one before GL_RGB so this should work.
		const auto format = GL_RGB + static_cast<int>(alpha);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, raw);

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(raw);
	}
	else
	{
		wmcv::LogMessage("Failed to load image: {}", path.string().c_str());
	}
}

void Texture::on(uint32_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::off()
{
	glActiveTexture(0);
}

} // namespace wmcv
