#include <render/Texture.hpp>
#include <thread>


#ifdef BUILD_WINDOWS

#include <core/Resources.hpp>
#include <render/opengl/GlFunctions.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <render/stb_image.h>
#include <render/stb_resize.h>

namespace volt {
	std::unordered_map<unsigned int, TexData> Texture::s_load_queue;
	int max_tex_size = 0;
	unsigned int count = 0;

	void loadTexture(const std::string& file, Texture* tex, unsigned int scale_factor) {
		stbi_set_flip_vertically_on_load(true);

		std::cout << "stbi loading " << file << std::endl;
		int bpp;
		void* buffer = stbi_load(file.c_str(), &tex->m_width, &tex->m_height, &bpp, 4);

		std::cout << "stbi load complete" << std::endl;

		int biggest = tex->m_width > tex->m_height ? tex->m_width : tex->m_height;

		if (biggest > max_tex_size) {
			scale_factor = biggest / max_tex_size;
		}

		if (scale_factor > 1) {

			int w2 = (tex->m_width / scale_factor);
			int h2 = (tex->m_height / scale_factor);

			void* temp = malloc((unsigned long long)w2 * h2 * 4);

			stbir_resize_uint8((unsigned char*)buffer, tex->m_width, tex->m_height, 0, (unsigned char*)temp, tex->m_width / scale_factor, tex->m_height / scale_factor, 0, 4);

			free(buffer);

			buffer = temp;

			tex->m_width /= scale_factor;
			tex->m_height /= scale_factor;
		}

		tex->m_gl_tex->load(buffer, { tex->m_width, tex->m_height });


		//TexData d;
		//d.tex = tex;
		//d.data = buffer;

		//Texture::s_load_queue[count] = d;

		//count = count == 100 ? 0 : count + 1;

		//std::cout << "stbi load cleanup" << std::endl;
	}

	Texture::Texture() : m_id(resources::assignId()), m_gl_tex(nullptr), m_width(1), m_height(1), m_is_initialised(false), m_is_loaded(false) {
		m_gl_tex = new GlTexture();
	}

	void Texture::init(float* data, iVec2 size) {
		m_gl_tex->load(data, size, true);
		m_is_loaded = true;
	}

	void Texture::init(const std::string& file, unsigned int scale_factor) {
		if (max_tex_size == 0) {
			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
		}
		m_is_initialised = true;
		//std::thread{ loadTexture,file,this, scale_factor }.detach();
		loadTexture(file, this, scale_factor);
	}

	void Texture::init(Vec4 colour) {
		m_is_initialised = true;

		m_gl_tex->load(colour);
	}

	Texture::Texture(const std::string& file, unsigned int scale_factor) : m_id(resources::assignId()), m_gl_tex(nullptr), m_width(1), m_height(1), m_is_initialised(true) {
		m_gl_tex = new GlTexture();

		init(file,scale_factor);
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

	bool Texture::isInitialised() {
		return m_is_initialised;
	}
}

#endif