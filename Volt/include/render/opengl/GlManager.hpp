#pragma once

namespace volt::gl {
	bool bindShader(unsigned int id);
	bool bindVertexArray(unsigned int id);
	bool bindVertexBuffer(unsigned int id);
	bool bindElementBuffer(unsigned int id);
	bool bindTexture(unsigned int id, unsigned int slot, unsigned int type = 0);
}
