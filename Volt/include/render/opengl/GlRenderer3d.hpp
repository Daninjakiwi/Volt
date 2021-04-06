#pragma once

#include <render/opengl/GlShader.hpp>
#include <render/Renderer3d.hpp>

namespace volt {
	struct DrawMesh {
		unsigned int start;
		unsigned int count;

		Material& material;
		Mat4 transform;

		DrawMesh(unsigned int start,unsigned int count,Material& material,Mat4 transform);
	};

	class GlRenderer3d : public Renderer3d {
	private:
		unsigned int m_vert_array, m_vert_buffer, m_elem_buffer;
		unsigned int m_tex_irradiance, m_tex_prefilter, m_tex_brdf, m_tex_cubemap;
		GlShader* m_shader;
		GlShader* m_background;
		Mat4 m_projection;
		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		std::vector<DrawMesh> m_draw_queue;
		Mat4 m_view;
	public:
		GlRenderer3d(float fov, float aspect_ratio, float near_plane, float far_plane);
		~GlRenderer3d();

		void setViewMatrix(Camera& cam);
		void drawMesh(Mesh& mesh, Material& material, Mat4 transform);
		void loadEnvironmentMap(float* data, iVec2 size);

		void renderFrame();
	private:
		void drawCube();
	};
}
