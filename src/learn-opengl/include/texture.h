#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <filesystem>

namespace wmcv
{
	struct Texture
	{
		Texture() = default;
		Texture(const std::filesystem::path& path, bool flip);

		void on(uint32_t slot);
		void off();

		uint32_t m_textureId;
	};
}

#endif //TEXTURE_H_INCLUDED
