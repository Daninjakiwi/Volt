#include <render/DynamicTexture.hpp>

namespace volt {
	Quality DynamicTexture::s_current = Quality::LOW;

	void DynamicTexture::setQuality(Quality quality) {
		s_current = quality;
	}

	DynamicTexture::DynamicTexture(const std::string& path) : m_path(path) {
	}

	DynamicTexture::operator Texture& () {
		if (!m_textures[(int)s_current].m_is_initialised) {
			m_textures[(int)s_current].init(m_path, (unsigned int)pow(2,(int)s_current));
		}
		return m_textures[(int)s_current];
	}
}