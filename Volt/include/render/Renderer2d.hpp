#pragma once
#include <util/maths/Vector.hpp>
#include <render/Quad.hpp>

namespace volt {
	class Renderer2d {
	public:
		Renderer2d(iVec2 size) {};
		virtual ~Renderer2d() {};

		virtual void drawQuad(Quad& quad, unsigned int flags) {};

		virtual void renderFrame() {};
	};
}
