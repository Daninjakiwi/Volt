#include <render/Texture.hpp>

#ifdef BUILD_WINDOWS

#include <core/Resources.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <render/stb_image.h>

namespace volt {
	Texture::Texture(const std::string& file) : m_id(resources::assignId()), m_gl_tex(nullptr) {
		stbi_set_flip_vertically_on_load(true);

		int width, height, bpp;
		void* buffer = stbi_load(file.c_str(), &width, &height, &bpp, 4);

		m_gl_tex = new GlTexture(buffer, { width,height });

		if (buffer) {
			stbi_image_free(buffer);
		}
	}

	Texture::~Texture() {
		delete m_gl_tex;
	}
}

#endif