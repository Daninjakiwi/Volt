#include <render/opengl/GlRenderer3d.hpp>
#include <render/opengl/GlShaderData.hpp>
#include <render/opengl/GlManager.hpp>
#include <render/opengl/GlFunctions.hpp>
#include <util/maths/Vector.hpp>

namespace volt {
	GlRenderer3d::GlRenderer3d(float fov, float aspect_ratio, float near_plane, float far_plane) : Renderer3d(), m_vert_array(0), m_vert_buffer(0), m_elem_buffer(0), m_shader(nullptr), m_projection() {
		m_shader = new GlShader(vert_3d, frag_3d);

		float tan_fov = maths::tand(fov / 2.0f);

		m_projection[0] = 1.0f / (aspect_ratio * tan_fov);
		m_projection[5] = 1.0f / tan_fov;
		m_projection[10] = -(far_plane + near_plane) / (far_plane - near_plane);
		m_projection[11] = -1.0f;
		m_projection[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);

		gl::bindShader(m_shader->getId());

		glUniformMatrix4fv(glGetUniformLocation(m_shader->getId(), "u_projection"), 1, false, &m_projection[0]);
		glUniform1i(glGetUniformLocation(m_shader->getId(), "u_albedo"), 1);

		glGenVertexArrays(1, &m_vert_array);
		glGenBuffers(1, &m_vert_buffer);
		glGenBuffers(1, &m_elem_buffer);

		gl::bindVertexArray(m_vert_array);
		gl::bindVertexBuffer(m_vert_buffer);
		gl::bindElementBuffer(m_elem_buffer);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), nullptr);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (const void*)12);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (const void*)24);
	}

	GlRenderer3d::~GlRenderer3d() {
		delete m_shader;

		glDeleteVertexArrays(1, &m_vert_array);
		glDeleteBuffers(1, &m_vert_buffer);
		glDeleteBuffers(1, &m_elem_buffer);
	}

	void GlRenderer3d::setViewMatrix(Mat4 view) {
		gl::bindShader(m_shader->getId());

		glUniformMatrix4fv(glGetUniformLocation(m_shader->getId(), "u_view"), 1, false, &view[0]);
	}

	void GlRenderer3d::renderFrame() {
		if (!m_draw_queue.empty()) {
			gl::bindShader(m_shader->getId());

			gl::bindVertexArray(m_vert_array);
			gl::bindVertexBuffer(m_vert_buffer);
			gl::bindElementBuffer(m_elem_buffer);

			glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 8 * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

			for (auto cmd : m_draw_queue) {
				unsigned int* offset = 0;
				offset += cmd.start;
				gl::bindTexture(cmd.material.getAlbedo().m_gl_tex->getId(), 1);
				glUniformMatrix4fv(glGetUniformLocation(m_shader->getId(), "u_model"), 1, false, &cmd.transform[0]);
				glDrawElements(GL_TRIANGLES, cmd.count, GL_UNSIGNED_INT, offset);
			}

			m_draw_queue.clear();
			m_vertices.clear();
			m_indices.clear();		
		}
	}

	DrawMesh::DrawMesh(unsigned int start, unsigned int count, Material& material, Mat4 transform) : start(start), count(count), material(material), transform(transform) {}

	void GlRenderer3d::drawMesh(Mesh& mesh, Material& material, Mat4 transform) {
		unsigned int start = m_indices.size();

		for (int i = 0; i < mesh.indices.size(); i++) {
			m_indices.push_back(mesh.indices[i] + m_vertices.size());
		}

		for (int i = 0; i < mesh.vertices.size(); i++) {
			m_vertices.push_back(mesh.vertices[i]);
		}

		m_draw_queue.push_back({ start, (unsigned int)mesh.indices.size(), material,transform });
	}
}