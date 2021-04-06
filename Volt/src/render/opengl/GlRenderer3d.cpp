#include <render/opengl/GlRenderer3d.hpp>
#include <render/opengl/GlShaderData.hpp>
#include <render/opengl/GlManager.hpp>
#include <render/opengl/GlFunctions.hpp>
#include <util/maths/Vector.hpp>

namespace volt {
	GlRenderer3d::GlRenderer3d(float fov, float aspect_ratio, float near_plane, float far_plane) : Renderer3d(), m_vert_array(0), m_vert_buffer(0), m_elem_buffer(0),m_tex_irradiance(0), m_tex_prefilter(0), m_tex_brdf(0),m_tex_cubemap(0), m_shader(nullptr),m_background(nullptr), m_projection() {
		m_shader = new GlShader(vert_pbr, frag_pbr);
		m_background = new GlShader(vert_background, frag_background);

		float tan_fov = maths::tand(fov / 2.0f);

		m_projection[0] = 1.0f / (aspect_ratio * tan_fov);
		m_projection[5] = 1.0f / tan_fov;
		m_projection[10] = -(far_plane + near_plane) / (far_plane - near_plane);
		m_projection[11] = -1.0f;
		m_projection[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);

		gl::bindShader(m_shader->getId());

		glUniformMatrix4fv(glGetUniformLocation(m_shader->getId(), "u_projection"), 1, false, &m_projection[0]);
		glUniform1i(glGetUniformLocation(m_shader->getId(), "u_albedo"), 1);
		glUniform1i(glGetUniformLocation(m_shader->getId(), "u_normal"), 2);
		glUniform1i(glGetUniformLocation(m_shader->getId(), "u_metallic"), 3);
		glUniform1i(glGetUniformLocation(m_shader->getId(), "u_roughness"), 4);
		glUniform1i(glGetUniformLocation(m_shader->getId(), "u_ao"), 5);

		glUniform1i(glGetUniformLocation(m_shader->getId(), "u_irradiance"), 6);
		glUniform1i(glGetUniformLocation(m_shader->getId(), "u_prefilter"), 7);
		glUniform1i(glGetUniformLocation(m_shader->getId(), "u_brdf"), 8);

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

		glGenTextures(1, &m_tex_irradiance);
		glGenTextures(1, &m_tex_prefilter);
		glGenTextures(1, &m_tex_brdf);
		glGenTextures(1, &m_tex_cubemap);
	}

	GlRenderer3d::~GlRenderer3d() {
		delete m_shader;

		glDeleteVertexArrays(1, &m_vert_array);
		glDeleteBuffers(1, &m_vert_buffer);
		glDeleteBuffers(1, &m_elem_buffer);

		glDeleteTextures(1, &m_tex_irradiance);
		glDeleteTextures(1, &m_tex_prefilter);
		glDeleteTextures(1, &m_tex_brdf);
		glDeleteTextures(1, &m_tex_cubemap);
	}

	void GlRenderer3d::setViewMatrix(Camera& cam) {
		gl::bindShader(m_shader->getId());

		glUniformMatrix4fv(glGetUniformLocation(m_shader->getId(), "u_view"), 1, false, &cam.getView()[0]);
		//glUniform3f(glGetUniformLocation(m_shader->getId(), "u_cam_pos"), view[12], view[13], view[14]);
		glUniform3f(glGetUniformLocation(m_shader->getId(), "u_cam_pos"), cam.m_position.x, cam.m_position.y, cam.m_position.z);

		//m_view = view;

		//std::cout << cam.m_position << std::endl;
	}

	void GlRenderer3d::renderFrame() {

		if (!m_draw_queue.empty()) {
			gl::bindShader(m_shader->getId());

			gl::bindTexture(m_tex_irradiance, 6, 1);
			gl::bindTexture(m_tex_prefilter, 7, 1);
			gl::bindTexture(m_tex_brdf, 8);


			gl::bindVertexArray(m_vert_array);
			gl::bindVertexBuffer(m_vert_buffer);
			gl::bindElementBuffer(m_elem_buffer);

			glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 8 * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

			for (auto cmd : m_draw_queue) {
				unsigned int* offset = 0;
				offset += cmd.start;
				gl::bindTexture(cmd.material.getAlbedo().m_gl_tex->getId(), 1);
				gl::bindTexture(cmd.material.getNormal().m_gl_tex->getId(), 2);
				gl::bindTexture(cmd.material.getMetallic().m_gl_tex->getId(), 3);
				gl::bindTexture(cmd.material.getRoughness().m_gl_tex->getId(), 4);
				gl::bindTexture(cmd.material.getAo().m_gl_tex->getId(), 5);
				glUniformMatrix4fv(glGetUniformLocation(m_shader->getId(), "u_model"), 1, false, &cmd.transform[0]);
				glDrawElements(GL_TRIANGLES, cmd.count, GL_UNSIGNED_INT, offset);
				//drawCube();

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

	void GlRenderer3d::loadEnvironmentMap(float* data, iVec2 size) {

		int original_viewport[4];

		glGetIntegerv(GL_VIEWPORT, original_viewport);

		const int hdr_size = 512;

		GlShader equi_to_cubemap(vert_cubemap, frag_equi_to_cubemap);
		GlShader irradiance_convolution(vert_cubemap, frag_irradiance_convolute);
		GlShader prefilter(vert_cubemap, frag_prefilter);
		GlShader brdf(vert_brdf, frag_brdf);

		unsigned int capture_framebuffer;
		unsigned int capture_renderbuffer;
		glGenFramebuffers(1, &capture_framebuffer);
		glGenRenderbuffers(1, &capture_renderbuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, capture_framebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, capture_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, hdr_size, hdr_size);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, capture_renderbuffer);

		unsigned int tex_hdri;
		glGenTextures(1, &tex_hdri);
		gl::bindTexture(tex_hdri, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, size.width, size.height, 0, GL_RGB, GL_FLOAT, data);

		unsigned int tex_env_cubemap;
		glGenTextures(1, &tex_env_cubemap);
		gl::bindTexture(tex_env_cubemap, 2, 1);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		for (unsigned int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, hdr_size, hdr_size, 0, GL_RGB, GL_FLOAT, nullptr);
		}

		Mat4 capture_projection = perspective(90.0f, 1.0f, 0.1f, 10.0f); //far can be 2
		Mat4 capture_views[6] = {
			lookAt({0.0f,0.0f,0.0f}, {1.0f,0.0f,0.0f}, {0.0f,-1.0f,0.0f}),
			lookAt({0.0f,0.0f,0.0f}, {-1.0f,0.0f,0.0f}, {0.0f,-1.0f,0.0f}),

			lookAt({0.0f,0.0f,0.0f}, {0.0f,1.0f,0.0f}, {0.0f,0.0f,1.0f}),
			lookAt({0.0f,0.0f,0.0f}, {0.0f,-1.0f,0.0f}, {0.0f,0.0f,-1.0f}),

			lookAt({0.0f,0.0f,0.0f}, {0.0f,0.0f,1.0f}, {0.0f,-1.0f,0.0f}),
			lookAt({0.0f,0.0f,0.0f}, {0.0f,0.0f,-1.0f}, {0.0f,-1.0f,0.0f})
		};

		gl::bindShader(equi_to_cubemap.getId());
		glUniform1i(glGetUniformLocation(equi_to_cubemap.getId(), "u_equi_map"), 1);
		glUniformMatrix4fv(glGetUniformLocation(equi_to_cubemap.getId(), "u_projection"), 1, false, &capture_projection[0]);

		glViewport(0, 0, hdr_size, hdr_size);

		for (unsigned int i = 0; i < 6; i++) {
			glUniformMatrix4fv(glGetUniformLocation(equi_to_cubemap.getId(), "u_view"), 1, false, &capture_views[i][0]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, tex_env_cubemap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			drawCube();
		}

		glDeleteTextures(1, &tex_hdri);

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		//IRRADIANCE

		gl::bindTexture(m_tex_irradiance, 1, 1);

		for (unsigned int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindFramebuffer(GL_FRAMEBUFFER, capture_framebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, capture_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

		gl::bindShader(irradiance_convolution.getId());
		glUniform1i(glGetUniformLocation(irradiance_convolution.getId(), "u_env_map"), 2);
		glUniformMatrix4fv(glGetUniformLocation(irradiance_convolution.getId(), "u_projection"), 1, false, &capture_projection[0]);

		glViewport(0, 0, 32, 32);

		for (unsigned int i = 0; i < 6; ++i) {
			glUniformMatrix4fv(glGetUniformLocation(irradiance_convolution.getId(), "u_view"), 1, false, &capture_views[i][0]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_tex_irradiance, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			drawCube();
		}

		//IRRADIANCE END

		gl::bindTexture(m_tex_prefilter, 3, 1);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		for (unsigned int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, hdr_size / 4, hdr_size / 4, 0, GL_RGB, GL_FLOAT, nullptr);
		}

		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		gl::bindShader(prefilter.getId());

		glUniform1i(glGetUniformLocation(prefilter.getId(), "u_env_map"), 2);
		glUniformMatrix4fv(glGetUniformLocation(prefilter.getId(), "u_projection"), 1, false, &capture_projection[0]);

		unsigned int mip_levels = 5;
		for (unsigned int i = 0; i < mip_levels; i++) {
			unsigned int width = (hdr_size / 4) * std::pow(0.5, i);
			unsigned int height = (hdr_size / 4) * std::pow(0.5, i);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
			glViewport(0, 0, width, height);

			float roughness = (float)i / (float)(mip_levels - 1);
			glUniform1f(glGetUniformLocation(prefilter.getId(), "u_roughness"), roughness);
			for (unsigned int j = 0; j < 6; j++) {
				glUniformMatrix4fv(glGetUniformLocation(prefilter.getId(), "u_view"), 1, false, &capture_views[j][0]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, m_tex_prefilter, i);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawCube();
			}
		}

		glDeleteTextures(1, &tex_env_cubemap);

		//Irradiance = 1
		//BRDF = 2
		//Prefilter = 3
		gl::bindTexture(m_tex_brdf, 2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, hdr_size, hdr_size, 0, GL_RG, GL_FLOAT, 0);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, hdr_size, hdr_size);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex_brdf, 0);

		glViewport(0, 0, hdr_size, hdr_size);
		gl::bindShader(brdf.getId());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		unsigned int va, vb;

		glGenVertexArrays(1, &va);
		glGenBuffers(1, &vb);

		gl::bindVertexArray(va);
		gl::bindVertexBuffer(vb);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDeleteVertexArrays(1, &va);
		glDeleteBuffers(1, &vb);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &capture_framebuffer);
		glDeleteRenderbuffers(1, &capture_renderbuffer);

		glViewport(original_viewport[0], original_viewport[1], original_viewport[2], original_viewport[3]);

	}

	void GlRenderer3d::drawCube() {
		unsigned int va, vb;
		glGenVertexArrays(1, &va);
		glGenBuffers(1, &vb);

		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};

		gl::bindVertexArray(va);
		gl::bindVertexArray(vb);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));


		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDeleteVertexArrays(1, &va);
		glDeleteBuffers(1, &vb);

	}
}