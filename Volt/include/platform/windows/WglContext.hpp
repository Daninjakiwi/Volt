#pragma once
#ifdef BUILD_WINDOWS
#include <core/GraphicsContext.hpp>
#include <platform/windows/WindowsWindow.hpp>

namespace volt {
	class WglContext : public GraphicsContext {
	private:
		HDC m_device;
		HGLRC m_context;
	public:
		WglContext(Window* window);
		~WglContext();

		void setBackgroundColour(Vec4 colour);

		void drawQuad2D(Vec2 pos, Vec2 size, Vec4 colour);

		void makeCurrent();

		void clear();
	};
}

#endif