#include <algorithm>
#include <chrono>
#include <thread>

#include <render/opengl/GlRenderer2d.hpp>

#include <render/opengl/GlFunctions.hpp>

#include "render/opengl/GlManager.hpp"

namespace volt {
	const unsigned long long quad_size = 6 * 4 * sizeof(float);

	const char* test_vert = 
		"#version 330 core\n"
		"layout(location = 0) in vec2 position;\n"
		"layout(location = 1) in vec4 colour;\n"
		"out vec4 v_colour;\n"
		"uniform mat4 u_projection;\n"
		"void main() {\n"
		"v_colour = colour;\n"
		"gl_Position = u_projection * vec4(position, 0.0, 1.0);\n"
		"}";

	const char* test_frag = 
		"#version 330 core\n"
		"layout(location = 0) out vec4 colour;\n"
		"in vec4 v_colour;\n"
		"void main() {\n"
		"colour = v_colour;\n"
		"}";

	DrawCommand::DrawCommand(unsigned long long start, unsigned long long count) : start(start), count(count) {}


	GlRenderer2d::GlRenderer2d(iVec2 size) : Renderer2d(size), m_shader(nullptr), m_vert_array(0), m_vert_buffer(0), m_elem_buffer(0), m_count((unsigned long long)1'048'576) {
		m_shader = new GlShader(test_vert, test_frag);
		m_projection = (float*)calloc(16, sizeof(float));

		if (m_projection != nullptr) {
			m_projection[0] = 2.0f / ((float)size.x);
			m_projection[5] = 2.0f / ((float)size.y);
			m_projection[10] = -1.0f;
			m_projection[12] = -1.0f;
			m_projection[13] = -1.0f;
			m_projection[15] = 1.0f;
		}

		unsigned int test;

		glCreateVertexArrays(1, &m_vert_array);
		glCreateVertexArrays(1, &test);
		glCreateBuffers(1, &m_vert_buffer);
		glCreateBuffers(1, &m_elem_buffer);

		glBindVertexArray(m_vert_array);
		glBindBuffer(GL_ARRAY_BUFFER, m_vert_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elem_buffer);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 2, GL_FLOAT, false, 6 * sizeof(float), nullptr);
		glVertexAttribPointer(1, 4, GL_FLOAT, false, 6 * sizeof(float), (const void*)8);

		glBindVertexArray(test);
		gl::bindVertexArray(m_vert_array);

		glBufferData(GL_ARRAY_BUFFER, quad_size * m_count, nullptr, GL_DYNAMIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int) * m_count, nullptr, GL_DYNAMIC_DRAW);

		glDeleteVertexArrays(1,&test);

		m_shader->bind();

		glUniformMatrix4fv(glGetUniformLocation(m_shader->getId(), "u_projection"), 1, false, m_projection);
	}

	GlRenderer2d::~GlRenderer2d() {
		delete m_shader;

		glDeleteVertexArrays(1, &m_vert_array);
		glDeleteBuffers(1, &m_vert_buffer);
		glDeleteBuffers(1, &m_elem_buffer);
	}

	void GlRenderer2d::drawQuad(Quad& quad, unsigned int flags) {
		gl::bindVertexArray(m_vert_array);
		gl::bindVertexBuffer(m_vert_buffer);

		if (m_locations.size() == m_count) {
			gl::bindElementBuffer(m_elem_buffer);

			void* data = glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);

			void* temp = malloc(quad_size * m_count * (unsigned long long)2u);

			if (temp) {
				memcpy(temp, data, quad_size * m_count);
			}

			glUnmapBuffer(GL_ARRAY_BUFFER);

			glBufferData(GL_ARRAY_BUFFER, quad_size * m_count * 2, temp, GL_DYNAMIC_DRAW);

			free(temp);

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

		bool f = flags == 48 ? false : true;

		if (!m_locations[quad.id]) {
			f = true;
			m_locations[quad.id] = (unsigned int)m_locations.size();

			gl::bindElementBuffer(m_elem_buffer);
			void* data = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, (m_locations.size() - 1) * 6 * sizeof(unsigned int), 6 * sizeof(unsigned int), GL_MAP_WRITE_BIT);

			unsigned long long offset = (m_locations.size() - (unsigned long long)1u) * (unsigned long long)4u;

			unsigned int indices[6] = {
				(unsigned int)offset + 0u,(unsigned int)offset + 1u,(unsigned int)offset + 2u,(unsigned int)offset + 2u,(unsigned int)offset + 3u, (unsigned int)offset + 0u
			};

			memcpy(data, indices, (unsigned long long)6u * sizeof(unsigned int));

			glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		}

		unsigned long long index = m_locations[quad.id] - 1;

		if (f) {
			void* data = glMapBufferRange(GL_ARRAY_BUFFER, index * quad_size, quad_size, GL_MAP_WRITE_BIT);

			float vertices[24] = {
				quad.pos.x, quad.pos.y, quad.colour.r,quad.colour.g, quad.colour.b,quad.colour.a,
				quad.pos.x + quad.size.x, quad.pos.y, quad.colour.r, quad.colour.g,quad.colour.b,quad.colour.a,
				quad.pos.x + quad.size.x, quad.pos.y + quad.size.y, quad.colour.r,quad.colour.g,quad.colour.b,quad.colour.a,
				quad.pos.x,quad.pos.y + quad.size.y, quad.colour.r,quad.colour.g,quad.colour.b,quad.colour.a
			};

			memcpy(data, vertices, quad_size);

			glUnmapBuffer(GL_ARRAY_BUFFER);
		}

		if (m_draw_buffer.size() > 0) {
			unsigned long long back = m_draw_buffer.back();
			if (back + (unsigned long long)1u == index) {
				m_draw_buffer.push_back(index);
			}
			else if (!std::binary_search(m_draw_buffer.begin(), m_draw_buffer.end(), index)) {
				m_draw_queue.emplace_back((unsigned long long)6u * m_draw_buffer[0] * sizeof(unsigned int), (unsigned long long)6u * m_draw_buffer.size());
				m_draw_buffer.clear();

				m_draw_buffer.push_back(index);
			}
		
		}
		else {
			m_draw_buffer.push_back(index);
		}
	}

	void GlRenderer2d::renderFrame() {			
		if (m_draw_buffer.size() > 0) {
				m_draw_queue.emplace_back((unsigned long long)6u * m_draw_buffer[0] * sizeof(unsigned int), (unsigned long long)6u * m_draw_buffer.size());
				m_draw_buffer.clear();
		}
		

		if (m_draw_queue.size() > 0) {
			gl::bindVertexArray(m_vert_array);

			gl::bindVertexBuffer(m_vert_buffer);
			gl::bindElementBuffer(m_elem_buffer);

			gl::bindShader(m_shader->getId());

			for (auto cmd : m_draw_queue) {
				glDrawElements(GL_TRIANGLES, (GLsizei)cmd.count, GL_UNSIGNED_INT, (void*)cmd.start);
			}

			m_draw_queue.clear();
		}	
	}
}