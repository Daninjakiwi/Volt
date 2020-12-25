#pragma once

#include <string>
#include <vector>

#ifdef BUILD_WINDOWS

#include <render/opengl/GlTexture.hpp>

namespace volt {
	struct TexData;

	class Texture {
		friend class GlRenderer2d;
		friend class WglContext;
		friend void loadTexture(const std::string& file, Texture* tex);
	private:
		int m_width, m_height;
		unsigned long long m_id;
		GlTexture* m_gl_tex;
	public:
		static std::vector<TexData> s_load_queue;

		Texture(const std::string& file);
		~Texture();

		int getWidth();
		int getHeight();
	};

	struct TexData {
		Texture* tex;
		void* data;
	};
}

#endif
