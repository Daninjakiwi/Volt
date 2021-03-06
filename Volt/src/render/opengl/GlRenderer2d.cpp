#include <algorithm>

#include <render/opengl/GlRenderer2d.hpp>

#include <render/opengl/GlFunctions.hpp>

#include <render/opengl/GlManager.hpp>

#include <core/VoltDefines.hpp>

#include <render/opengl/GlShaderData.hpp>

#include <core/Font.hpp>

namespace volt {

	const unsigned long long quad_size = 9 * 4 * sizeof(float);
	const unsigned long long indices_size = 6 * sizeof(unsigned int);

	DrawCommand::DrawCommand(unsigned long long start, unsigned long long count) : m_start(start), m_count(count) {}

	void DrawCommand::execute() {
		glDrawElements(GL_TRIANGLES, (GLsizei)m_count, GL_UNSIGNED_INT, (void*)m_start);
	}

	BindTextureCommand::BindTextureCommand(unsigned int id, unsigned int slot) : m_id(id), m_slot(slot) {}

	void BindTextureCommand::execute() {
		gl::bindTexture(m_id, m_slot);
	}


	GlRenderer2d::GlRenderer2d(iVec2 size) : Renderer2d(size), m_shader(nullptr), m_vert_array(0), m_vert_buffer(0), m_elem_buffer(0), m_count((unsigned long long)262144) {
		m_shader = new GlShader(vert_2d, frag_2d);
		m_projection = (float*)calloc(16, sizeof(float));

		if (m_projection != nullptr) {
			m_projection[0] = 2.0f / ((float)size.x);
			m_projection[5] = 2.0f / ((float)size.y);
			m_projection[10] = -1.0f;
			m_projection[12] = -1.0f;
			m_projection[13] = -1.0f;
			m_projection[15] = 1.0f;
		}

		glGenVertexArrays(1, &m_vert_array);
		glGenBuffers(1, &m_vert_buffer);
		glGenBuffers(1, &m_elem_buffer);

		gl::bindVertexArray(m_vert_array);
		gl::bindVertexBuffer(m_vert_buffer);
		gl::bindElementBuffer(m_elem_buffer);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 2, GL_FLOAT, false, 9 * sizeof(float), nullptr);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, 9 * sizeof(float), (const void*)8);
		glVertexAttribPointer(2, 4, GL_FLOAT, false, 9 * sizeof(float), (const void*)16);
		glVertexAttribPointer(3, 1, GL_FLOAT, false, 9 * sizeof(float), (const void*)32);

		glBufferData(GL_ARRAY_BUFFER, quad_size * m_count, nullptr, GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int) * m_count, nullptr, GL_DYNAMIC_DRAW);

		gl::bindShader(m_shader->getId());

		glUniformMatrix4fv(glGetUniformLocation(m_shader->getId(), "u_projection"), 1, false, m_projection);

		

		for (int i = 1; i <= 16; i++) {
			unsigned int location = glGetUniformLocation(m_shader->getId(), ("u_textures[" + std::to_string(i-1) + "]").c_str());
			glUniform1i(location, i);
		}

		m_delete_count = 0;
		m_quad_count = 0;
		m_first = 0;
		m_last = 0;
		m_is_empty = true;
	}

	GlRenderer2d::~GlRenderer2d() {
		delete m_shader;

		free(m_projection);

		glDeleteVertexArrays(1, &m_vert_array);
		glDeleteBuffers(1, &m_vert_buffer);
		glDeleteBuffers(1, &m_elem_buffer);
	}

	void GlRenderer2d::drawTexture(Texture& tex, Vec2 pos, Vec2 size) {
		Vec2 coords[4] = { {0.0f,0.0f}, {1.0f,0.0f}, {1.0f,1.0f}, {0.0f,1.0f} };
		drawTexture(tex, pos, size, coords);
	}

	void GlRenderer2d::drawTexture(Texture& tex, Vec2 pos, Vec2 size, Vec2* coords) {
		drawTexture(tex, pos, size, coords, {0.0f,0.0f,0.0f,0.0f});
	}

	void GlRenderer2d::drawString(const std::string& text, Vec2 pos, unsigned int size, unsigned long long font, Vec4 colour) {
		Font* f = Font::get(font);

		int offset = 0;

		float scale = (float)size / (float)f->m_font_size;

		float l = (float)f->m_line_height - (float)f->m_font_height;

		float line_height = (l + ((float)f->m_font_height / 2.0f)) * scale;


		for (unsigned char c : text) {
			Glyph g = f->m_characters[c];

			g.w *= scale;
			g.h *= scale;
			g.offsetx *= scale;
			g.offsety *= scale;
			g.advance *= scale;

			Vec2 char_pos(pos.x + offset + g.offsetx, pos.y - g.offsety + line_height);
			Vec2 size(g.w, -g.h);

			Vec2 coords[4] = { {g.xr, g.yr + g.hr}, {g.xr + g.wr, g.yr + g.hr}, {g.xr + g.wr, g.yr}, {g.xr, g.yr} };
			drawTexture(f->m_tex, char_pos, size, coords, colour);
	
			offset += g.advance;
		}

	}

	void GlRenderer2d::insertIndices(unsigned long long index, unsigned int* indices) {
		gl::bindElementBuffer(m_elem_buffer);


		void* data = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, index * indices_size, indices_size, GL_MAP_WRITE_BIT);

		memcpy(data,indices, 6 * sizeof(unsigned int));

		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}

	void GlRenderer2d::insertVertices(unsigned long long index, float* vertices) {
		gl::bindVertexBuffer(m_vert_buffer);

		float* data = (float*)glMapBufferRange(GL_ARRAY_BUFFER, index * quad_size, quad_size, GL_MAP_WRITE_BIT);

		memcpy(data, vertices, quad_size);

		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	void GlRenderer2d::drawQuad(Quad& quad, unsigned int flags) {
		bool update_vertices = flags & NO_UPDATE ? false : true;

		unsigned long long index;

		//Check if buffer needs to be expanded
		if (m_quad_count == m_count) {
			//Copy data into local memory and then insert into new buffer
			void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

			void* temp = malloc(quad_size * m_count * (unsigned long long)2u);

			if (temp) {
				memcpy(temp, data, quad_size * m_count);
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);

			glBufferData(GL_ARRAY_BUFFER, quad_size * m_count * 2, temp, GL_DYNAMIC_DRAW);

			free(temp);

			gl::bindElementBuffer(m_elem_buffer);

			data = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);

			temp = malloc((unsigned long long)12u * sizeof(unsigned int) * m_count);

			if (temp) {
				memcpy(temp, data, (unsigned long long)6u * sizeof(unsigned int) * m_count);
			}

			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned long long)12u * sizeof(unsigned int) * m_count, temp, GL_DYNAMIC_DRAW);

			free(temp);

			m_count *= 2;
		}

		//Check if quad needs to be added to buffer
		if (m_locations_map[quad.id].size() == 0 || flags & SEPERATE) {
			update_vertices = true;
			index = m_quad_count;

			m_locations_map[quad.id].push_back(m_quad_count);

			unsigned int offset = m_quad_count * 4;

			unsigned int indices[6] = {
				0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset
			};

			insertIndices(m_quad_count, indices);

			m_quad_count++;
			m_delete_count++;
		}
		else {
			index = m_locations_map[quad.id][0];
		}

		if (update_vertices) {

			float vertices[36] = {
			quad.pos.x, quad.pos.y,0.0f, 0.0f, quad.colour.r, quad.colour.g, quad.colour.b,quad.colour.a, 0.0f,
			quad.pos.x + quad.size.x, quad.pos.y,0.0f, 0.0f, quad.colour.r, quad.colour.g,quad.colour.b,quad.colour.a, 0.0f,
			quad.pos.x + quad.size.x, quad.pos.y + quad.size.y,0.0f, 0.0f, quad.colour.r,quad.colour.g,quad.colour.b,quad.colour.a, 0.0f,
			quad.pos.x,quad.pos.y + quad.size.y,0.0f, 0.0f, quad.colour.r,quad.colour.g,quad.colour.b,quad.colour.a, 0.0f
			};

			insertVertices(index, vertices);
		}

		if (index == m_last + 1 && !m_is_empty) {
			m_last++;
		}
		else {
			if (!(index >= m_first && index <= m_last && !m_is_empty)) {
				if (!m_is_empty) {
					DrawCommand* cmd = new DrawCommand(m_first * indices_size, (m_last - m_first + 1) * 6);

					m_command_queue.push_back(cmd);
				}

				m_first = index;
				m_last = index;
				m_is_empty = false;
			}
		}
	}

	void GlRenderer2d::drawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Vec4 colour) {
		unsigned long long index;

		//Check if buffer needs to be expanded
		if (m_quad_count == m_count) {
			//Copy data into local memory and then insert into new buffer
			void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

			void* temp = malloc(quad_size * m_count * (unsigned long long)2u);

			if (temp) {
				memcpy(temp, data, quad_size * m_count);
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);

			glBufferData(GL_ARRAY_BUFFER, quad_size * m_count * 2, temp, GL_DYNAMIC_DRAW);

			free(temp);

			gl::bindElementBuffer(m_elem_buffer);

			data = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);

			temp = malloc((unsigned long long)12u * sizeof(unsigned int) * m_count);

			if (temp) {
				memcpy(temp, data, (unsigned long long)6u * sizeof(unsigned int) * m_count);
			}

			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned long long)12u * sizeof(unsigned int) * m_count, temp, GL_DYNAMIC_DRAW);

			free(temp);

			m_count *= 2;
		}

		//Check if quad needs to be added to buffer
		index = m_quad_count;

		m_locations_map[0].push_back(m_quad_count);

		unsigned int offset = m_quad_count * 4;

		unsigned int indices[6] = {
			0 + offset, 1 + offset, 2 + offset, 0 + offset, 1 + offset, 2 + offset
		};

		insertIndices(m_quad_count, indices);

		m_quad_count++;
		m_delete_count++;

		float vertices[36] = {
			p1.x, p1.y,0.0f, 0.0f, colour.r, colour.g,colour.b,colour.a, 0.0f,
			p2.x, p2.y,0.0f, 0.0f, colour.r, colour.g,colour.b,colour.a, 0.0f,
			p3.x, p3.y,0.0f, 0.0f, colour.r,colour.g,colour.b,colour.a, 0.0f,
			0.0f,0.0f,0.0f, 0.0f, colour.r,colour.g,colour.b,colour.a, 0.0f
		};

		insertVertices(index, vertices);

		if (index == m_last + 1 && !m_is_empty) {
			m_last++;
		}
		else {
			if (!(index >= m_first && index <= m_last && !m_is_empty)) {
				if (!m_is_empty) {
					DrawCommand* cmd = new DrawCommand(m_first * indices_size, (m_last - m_first + 1) * 6);

					m_command_queue.push_back(cmd);
				}

				m_first = index;
				m_last = index;
				m_is_empty = false;
			}
		}
	}

	void GlRenderer2d::drawTexture(Texture& tex, Vec2 pos, Vec2 size, Vec2* coords, Vec4 filter_colour) {
		//CHECK IF TEXTURE IS IN MAP

		m_delete_count++;

		unsigned int slot = m_texture_map[tex.m_id];
		unsigned long long index;

		if (!slot) {
			if (m_texture_map.size() <= 16) {
				//THERE IS SPACE
				slot = m_texture_map.size();
			}
			else {
				//NO SPACE
				slot = 1;

				unsigned int replaced = m_texture_map_invert[slot];

				m_texture_map.erase(replaced);

				//Check if replaced texture is in current draw queue
				if (replaced >= m_first && replaced <= m_last && !m_is_empty) {
					//FLUSH DRAW QUEUE
					DrawCommand* cmd = new DrawCommand(m_first * indices_size, (m_last - m_first + 1) * 6);

					m_command_queue.push_back(cmd);

					m_first = 0;
					m_last = 0;
					m_is_empty = true;
				}

			}
			m_texture_map[tex.m_id] = slot;
			m_texture_map_invert[slot] = tex.m_id;

			BindTextureCommand* cmd = new BindTextureCommand(tex.m_gl_tex->getId(), slot);
			m_command_queue.push_back(cmd);
		}	

		if (m_quad_count == m_count) {
			//Copy data into local memory and then insert into new buffer
			void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

			void* temp = malloc(quad_size * m_count * (unsigned long long)2u);

			if (temp) {
				memcpy(temp, data, quad_size * m_count);
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);

			glBufferData(GL_ARRAY_BUFFER, quad_size * m_count * 2, temp, GL_DYNAMIC_DRAW);

			free(temp);

			gl::bindElementBuffer(m_elem_buffer);

			data = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);

			temp = malloc((unsigned long long)12u * sizeof(unsigned int) * m_count);

			if (temp) {
				memcpy(temp, data, (unsigned long long)6u * sizeof(unsigned int) * m_count);
			}

			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned long long)12u * sizeof(unsigned int) * m_count, temp, GL_DYNAMIC_DRAW);

			free(temp);

			m_count *= 2;
		}

		index = m_quad_count;

		m_locations_map[tex.m_id].push_back(m_quad_count);

		unsigned int offset = m_quad_count * 4;

		unsigned int indices[6] = {
			0 + offset, 1 + offset, 2 + offset, 2 + offset, 3 + offset, 0 + offset
		};

		insertIndices(m_quad_count, indices);

		m_quad_count++;

		float vertices[36] = {
			pos.x, pos.y, coords[0].x,coords[0].y, filter_colour.r,filter_colour.g,filter_colour.b,filter_colour.a,slot,
			pos.x + size.x, pos.y, coords[1].x,coords[1].y, filter_colour.r,filter_colour.g,filter_colour.b,filter_colour.a,slot,
			pos.x + size.x, pos.y + size.y, coords[2].x,coords[2].y, filter_colour.r,filter_colour.g,filter_colour.b,filter_colour.a,slot,
			pos.x, pos.y + size.y,coords[3].x,coords[3].y, filter_colour.r,filter_colour.g,filter_colour.b,filter_colour.a,slot
		};

		insertVertices(index, vertices);

		if (index == m_last + 1 && !m_is_empty) {
			m_last++;
		}
		else {
			if (!(index >= m_first && index <= m_last && !m_is_empty)) {
				if (!m_is_empty) {
					DrawCommand* cmd = new DrawCommand(m_first * indices_size, (m_last - m_first + 1) * 6);

					m_command_queue.push_back(cmd);
				}

				m_first = index;
				m_last = index;
				m_is_empty = false;
			}
		}

	}

	void GlRenderer2d::renderFrame() {
		if (!m_is_empty) {
			DrawCommand* cmd = new DrawCommand(m_first * indices_size, (m_last - m_first + 1) * 6);

			m_command_queue.push_back(cmd);

			m_is_empty = true;
		}

		if (m_command_queue.size() > 0) {
			gl::bindVertexArray(m_vert_array);
			gl::bindVertexBuffer(m_vert_buffer);
			gl::bindElementBuffer(m_elem_buffer);

			gl::bindShader(m_shader->getId());

			for (auto cmd : m_command_queue) {
				cmd->execute();
				delete cmd;
			}

			m_command_queue.clear();

			m_texture_map.clear();
			m_texture_map_invert.clear();
		}

		if (m_delete_count > m_count / 2 || (m_delete_count > m_count / 3 && m_quad_count >= m_count / 1.1)) {
			m_locations_map.clear();

			m_delete_count = 0;
			m_quad_count = 0;
			m_first = 0;
			m_last = 0;
			m_is_empty = true;
		}
	}
}