#include "render/opengl/GlTexture.hpp"

#include "render/opengl/GlFunctions.hpp"

namespace volt {
	GlTexture::GlTexture(void* data, iVec2 size) : m_id(0) {
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}

	GlTexture::~GlTexture() {
		glDeleteTextures(1, &m_id);
	}

	unsigned int GlTexture::getId() const {
		return m_id;
	}
}