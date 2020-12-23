#pragma once
#include <util/maths/Vector.hpp>
#include <render/Quad.hpp>
#include <render/Texture.hpp>

namespace volt {
	class Renderer2d {
	public:
		Renderer2d(iVec2 size) {};
		virtual ~Renderer2d() {};

		virtual void drawQuad(Quad& quad, unsigned int flags) {};
		virtual void drawTexture(Texture& tex, Vec2 pos, Vec2 size) {};

		virtual void renderFrame() {};
	};
}
