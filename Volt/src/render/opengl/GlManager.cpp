#include "render/opengl/GlManager.hpp"

#include "render/opengl/GlFunctions.hpp"

namespace volt::gl {

	int shader,va,vb,eb = -1;

	bool bindShader(unsigned int id) {
		if (shader != id) {
			glUseProgram(id);
			shader = id;
			return true;
		}
		return false;
	}

	bool bindVertexArray(unsigned int id) {
		if (va != id) {
			glBindVertexArray(id);
			va = id;
			return true;
		}
		return false;
	}

	bool bindVertexBuffer(unsigned int id) {
		if (vb != id) {
			glBindBuffer(GL_ARRAY_BUFFER, id);
			vb = id;
			return true;
		}
		return false;
	}

	bool bindElementBuffer(unsigned int id) {
		if (eb != id) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
			eb = id;
			return true;
		}
		return false;
	}
}