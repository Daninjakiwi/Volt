#include <render/Texture.hpp>
#include <thread>


#ifdef BUILD_WINDOWS

#include <core/Resources.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <render/stb_image.h>

namespace volt {
	std::vector<TexData> Texture::s_load_queue;

	void loadTexture(const std::string& file, Texture* tex) {
		stbi_set_flip_vertically_on_load(true);

		int bpp;
		void* buffer = stbi_load(file.c_str(), &tex->m_width, &tex->m_height, &bpp, 4);

		TexData d;
		d.tex = tex;
		d.data = buffer;

		Texture::s_load_queue.push_back(d);
	}

	Texture::Texture(const std::string& file) : m_id(resources::assignId()), m_gl_tex(nullptr), m_width(1), m_height(1) {
		m_gl_tex = new GlTexture();

		std::thread{ loadTexture,file,this }.detach();
	}

	Texture::~Texture() {
		delete m_gl_tex;
	}

	int Texture::getWidth() {
		return m_width;
	}

	int Texture::getHeight() {
		return m_height;
	}
}

#endif