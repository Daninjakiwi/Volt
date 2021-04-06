#pragma once
#ifdef BUILD_WINDOWS
#include <core/GraphicsContext.hpp>
#include <platform/windows/WindowsWindow.hpp>
#include <render/opengl/GlRenderer2d.hpp>
#include <render/opengl/GlRenderer3d.hpp>

namespace volt {
	class WglContext : public GraphicsContext {
	private:
		HDC m_device;
		HGLRC m_context;

		Texture m_default;

		Renderer2d* m_renderer_2d;
		Renderer3d* m_renderer_3d;
	public:
		WglContext(Window* window);
		~WglContext();

		void setBackgroundColour(Vec4 colour);

		void drawQuad(Quad& quad, unsigned int flags);
		void drawTexture(Texture& tex, Vec2 pos, Vec2 size);
		void drawString(const std::string& text, Vec2 pos, unsigned int size, unsigned long long font, Vec4 colour);
		void drawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Vec4 colour);
		void setViewMatrix(Camera& cam);
		void drawMesh(Mesh& mesh, Material& material, Mat4 transform);
		void loadEnvironmentMap(float* data, iVec2 size);

		void makeCurrent();

		void renderFrame();

		void clear();
	};
}

#endif