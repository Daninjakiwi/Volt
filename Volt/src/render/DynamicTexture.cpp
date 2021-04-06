#include <render/DynamicTexture.hpp>

namespace volt {
	Quality DynamicTexture::s_current = Quality::LOW;
	Texture DynamicTexture::s_default;

	void DynamicTexture::setQuality(Quality quality) {
		s_current = quality;
	}

	DynamicTexture::DynamicTexture(const std::string& path) : m_path(path)  {
	}

	DynamicTexture::operator Texture& () {
		if (!m_textures[(int)s_current].m_is_initialised) {
			m_textures[(int)s_current].init(m_path, (unsigned int)pow(2,(int)s_current));
		}

		if (!m_textures[(int)s_current].m_is_loaded) {
			if (!s_default.m_is_loaded) {
				//s_default.m_is_initialised = true;
				//s_default.m_is_loaded = true;
				//s_default.m_gl_tex->load({ 1.0f,0.0f,0.0f,1.0f });
			}
			//std::cout << "not ready" << std::endl;
			//return s_default;
		}

		return m_textures[(int)s_current];
	}
}