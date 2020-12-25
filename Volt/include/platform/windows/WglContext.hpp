#pragma once
#ifdef BUILD_WINDOWS
#include <core/GraphicsContext.hpp>
#include <platform/windows/WindowsWindow.hpp>
#include <render/opengl/GlRenderer2d.hpp>

namespace volt {
	class WglContext : public GraphicsContext {
	private:
		HDC m_device;
		HGLRC m_context;

		Renderer2d* m_renderer_2d;
	public:
		WglContext(Window* window);
		~WglContext();

		void setBackgroundColour(Vec4 colour);

		void drawQuad(Quad& quad, unsigned int flags);
		void drawTexture(Texture& tex, Vec2 pos, Vec2 size);
		void drawString(const std::string& text, Vec2 pos, unsigned int size, unsigned long long font, Vec4 colour);

		void makeCurrent();

		void renderFrame();

		void clear();
	};
}

#endif