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
	stbi_set_flip_vertically_on_load(static_cast<int>(flip));

	glGenTextures(1, &m_textureId);

	int32_t width, height, nrChannels;
	if (unsigned char* raw = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0); raw)
	{
		GLenum format = [](int32_t nrChannels)
		{
			if (nrChannels == 1)
				return GL_RED;
			else if (nrChannels == 3)
				return GL_RGB;
			else if (nrChannels == 4)
				return GL_RGBA;

			return 0;

		}(nrChannels);

		glBindTexture(GL_TEXTURE_2D, m_textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, raw);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
