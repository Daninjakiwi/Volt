#include <render/opengl/GlManager.hpp>

#include <render/opengl/GlFunctions.hpp>

namespace volt::gl {

	int shader,va,vb,eb = -1;

	unsigned int textures[16];

	bool bindShader(unsigned int id) {
		glUseProgram(id);
		if (shader != id) {
			
			shader = id;
			return true;
		}
		return false;
	}

	bool bindVertexArray(unsigned int id) {
		glBindVertexArray(id);
		if (va != id) {
			
			va = id;
			return true;
		}
		return false;
	}

	bool bindVertexBuffer(unsigned int id) {
		glBindBuffer(GL_ARRAY_BUFFER, id);
		if (vb != id) {
			
			vb = id;
			return true;
		}
		return false;
	}

	bool bindElementBuffer(unsigned int id) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
		if (eb != id) {
			
			eb = id;
			return true;
		}
		return false;
	}

	bool bindTexture(unsigned int id, unsigned int slot, unsigned int type) {
		glActiveTexture(GL_TEXTURE0 + slot);
		if (type == 1) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, id);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, id);
		}

		return true;

		/*
		if (textures[slot - 1] != id) {
			textures[slot - 1] = id;
			

			
			return true;
		}
		return false;

		*/
	}
}