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
		virtual void drawString(const std::string& text, Vec2 pos, unsigned int size, unsigned long long font, Vec4 colour) {};
		virtual void drawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Vec4 colour) {};

		virtual void renderFrame() {};
	};
}
