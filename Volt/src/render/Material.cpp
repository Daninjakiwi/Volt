#include <render/Material.hpp>

namespace volt {
	Texture s_tex_black;
	Texture s_tex_white;
	Texture s_tex_normal;

	ColourMaterial::ColourMaterial(Vec4 colour) {
		m_tex_colour.init(colour);
		if (!s_tex_black.isInitialised()) {
			s_tex_black.init(Vec4(0.0f,0.0f,0.0f,1.0f));
		}

		if (!s_tex_white.isInitialised()) {
			s_tex_white.init(Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}

		if (!s_tex_normal.isInitialised()) {
			s_tex_normal.init(Vec4(0.5f, 0.5f, 1.0f, 1.0f));
		}
	}

	Texture& ColourMaterial::getAlbedo() {
		return m_tex_colour;
	}
	Texture& ColourMaterial::getNormal() {
		return s_tex_normal;
	}
	Texture& ColourMaterial::getMetallic() {
		return s_tex_black;
	}
	Texture& ColourMaterial::getRoughness() {
		return s_tex_white;
	}
	Texture& ColourMaterial::getAo() {
		return s_tex_white;
	}
}