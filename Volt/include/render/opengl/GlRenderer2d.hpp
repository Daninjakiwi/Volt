#pragma once

#include <unordered_map>

#include <render/opengl/GlShader.hpp>
#include <render/Renderer2d.hpp>

namespace volt {
	struct DrawCommand {
		unsigned long long start;
		unsigned long long count;

		DrawCommand(unsigned long long start, unsigned long long count);
	};

	class GlRenderer2d : public Renderer2d {
	private:
		GlShader* m_shader;
		float* m_projection;
		unsigned int m_vert_array, m_vert_buffer, m_elem_buffer;
		std::unordered_map<unsigned long long, unsigned long long> m_locations;
		std::vector<unsigned long long> m_draw_buffer;
		std::vector<DrawCommand> m_draw_queue;
		unsigned long long m_count;

	public:
		GlRenderer2d(iVec2 size);
		~GlRenderer2d();

		void drawQuad(Quad& quad, unsigned int flags);
		void renderFrame();
	};
}
