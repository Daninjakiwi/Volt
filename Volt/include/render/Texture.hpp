#pragma once

#include <string>
#include <vector>

#ifdef BUILD_WINDOWS

#include <render/opengl/GlTexture.hpp>

namespace volt {
	struct TexData;

	class Texture {
		friend class GlRenderer2d;
		friend class GlRenderer3d;
		friend class WglContext;
		friend class DynamicTexture;
		friend void loadTexture(const std::string& file, Texture* tex, unsigned int scale_factor);
	private:
		int m_width, m_height;
		unsigned long long m_id;
		GlTexture* m_gl_tex;
		bool m_is_initialised;
	public:


		static std::vector<TexData> s_load_queue;

		Texture(const std::string& file, unsigned int scale_factor = 1);
		Texture();
		~Texture();

		int getWidth();
		int getHeight();

		bool isInitialised();

		void init(const std::string& file, unsigned int scale_factor = 1);
		void init(Vec4 colour);
	};

	struct TexData {
		Texture* tex;
		void* data;
	};
}

#endif
