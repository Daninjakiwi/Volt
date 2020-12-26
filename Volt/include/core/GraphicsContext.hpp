#pragma once
#include <render/Quad.hpp>
#include <render/Texture.hpp>

namespace volt {
	enum class ContextType {
		OPENGL, VULKAN, DIRECTX9, DIRECTX11, DIRECTX12
	};

	class GraphicsContext {
	public:
		GraphicsContext() {};
		virtual ~GraphicsContext() {};

		virtual void setBackgroundColour(Vec4 colour) {};

		virtual void drawQuad(Quad& quad, unsigned int flags) {};
		virtual void drawTexture(Texture& tex, Vec2 pos, Vec2 size) {};
		virtual void drawString(const std::string& text, Vec2 pos, unsigned int size, unsigned long long font, Vec4 colour) {};
		virtual void drawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Vec4 colour) {};

		virtual void makeCurrent() {};

		virtual void renderFrame() {};

		virtual void clear() {};
	};
}