#pragma once

#include <string>
#include <unordered_map>

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
		bool m_is_loaded;
	public:
		static std::unordered_map<unsigned int, TexData> s_load_queue;

		Texture(const std::string& file, unsigned int scale_factor = 1);
		Texture();
		~Texture();

		int getWidth();
		int getHeight();

		bool isInitialised();

		void init(const std::string& file, unsigned int scale_factor = 1);
		void init(Vec4 colour);
		void init(float* data, iVec2 size);
	};

	struct TexData {
		Texture* tex;
		void* data;
	};
}

#endif
