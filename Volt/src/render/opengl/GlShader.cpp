#include <render/opengl/GlShader.hpp>
#include <render/opengl/GlFunctions.hpp>

namespace volt {
	GlShader::GlShader(const char* vert_src, const char* frag_src) : m_id(0) {
		m_id = glCreateProgram();

		unsigned int vert = compile(vert_src, GL_VERTEX_SHADER);
		unsigned int frag = compile(frag_src, GL_FRAGMENT_SHADER);

		if (vert != 0 && frag != 0) {

			glAttachShader(m_id, vert);
			glAttachShader(m_id, frag);

			glLinkProgram(m_id);
			glValidateProgram(m_id);
		}
		glDeleteShader(vert);
		glDeleteShader(frag);
	}

	GlShader::~GlShader() {
		glDeleteProgram(m_id);
	}

	void GlShader::bind() {
		glUseProgram(m_id);
	}

	unsigned int GlShader::getId() {
		return m_id;
	}

	unsigned int GlShader::compile(const char* src, unsigned int type) {
		unsigned int id = glCreateShader(type);

		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE) {
			return 0;
		}

		return id;
	}
}