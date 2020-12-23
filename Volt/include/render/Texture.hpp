#pragma once

#include <string>

#ifdef BUILD_WINDOWS

#include <render/opengl/GlTexture.hpp>

namespace volt {
	class Texture {
		friend class GlRenderer2d;
	private:
		unsigned long long m_id;
		GlTexture* m_gl_tex;
	public:
		Texture(const std::string& file);
		~Texture();
	};
}

#endif
