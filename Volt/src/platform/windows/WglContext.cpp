#ifdef BUILD_WINDOWS
#include <platform/windows/WglContext.hpp>
#include <render/opengl/GlFunctions.hpp>
#include <render/opengl/GlManager.hpp>
#include <render/Camera.hpp>
#include <render/stb_resize.h>

namespace volt {

	float rot = 0.0f;

	WglContext::WglContext(Window* window) : m_device(nullptr), m_context(nullptr), m_renderer_2d(nullptr), m_renderer_3d(nullptr), m_default() {
		m_device = GetDC(window->m_handle);

		HGLRC temp_context = wglCreateContext(m_device);
		wglMakeCurrent(m_device, temp_context);

		float version = std::stof((const char*)glGetString(GL_VERSION));

		int major = 0;
		int minor = 0;

		if (version >= 4.5f) {
			major = 4;
			minor = 5;
		}
		else if (version >= 3.3f) {
			major = 3;
			minor = 3;
		}

		if (major != 0) {

			int attribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, major,
				WGL_CONTEXT_MINOR_VERSION_ARB, minor,
				WGL_CONTEXT_FLAGS_ARB, 0,
				WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0
			};

			PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
			PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
			wglSwapIntervalEXT(1);



			m_context = wglCreateContextAttribsARB(m_device, 0, attribs);

			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(temp_context);

			wglMakeCurrent(m_device, m_context);

			loadGlFunctions();

			glViewport(0, 0, window->m_size.x, window->m_size.y);

			int val;

			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &val);

			glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &val);

			glEnable(GL_DEPTH_TEST);

			//glDisable(GL_CULL_FACE);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS).

			if (major == 4) {
				m_renderer_2d = new GlRenderer2d(window->m_size);
				m_renderer_3d = new GlRenderer3d(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
			}
			else if (major == 3) {
				m_renderer_2d = new GlRenderer2d(window->m_size);
				m_renderer_3d = new GlRenderer3d(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
			}
		}

	}

	WglContext::~WglContext() {
		delete m_renderer_2d;
		delete m_renderer_3d;
	}

	void WglContext::setBackgroundColour(Vec4 colour) {
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void WglContext::drawQuad(Quad& quad, unsigned int flags) {
		if (m_renderer_2d) {
			m_renderer_2d->drawQuad(quad, flags);
		}
	}

	void WglContext::drawTexture(Texture& tex, Vec2 pos, Vec2 size) {
		if (m_renderer_2d) {
			if (tex.m_is_loaded) {
				m_renderer_2d->drawTexture(tex, pos, size);
			}
			else {
				m_renderer_2d->drawTexture(m_default, pos, size);
			}
			
		}
	}

	void WglContext::drawString(const std::string& text, Vec2 pos, unsigned int size, unsigned long long font, Vec4 colour) {
		if (m_renderer_2d) {
			m_renderer_2d->drawString(text, pos, size, font, colour);
		}
	}

	void WglContext::drawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Vec4 colour) {
		if (m_renderer_2d) {
			m_renderer_2d->drawTriangle(p1, p2, p3, colour);
		}
	}

	void WglContext::makeCurrent() {
		wglMakeCurrent(m_device, m_context);
	}



	void WglContext::renderFrame() {
		/*
		while (!Texture::s_load_queue.empty()) {
			std::cout << "textures to be loaded" << std::endl;
			for (int i = 0; i < 100; i++) {
				if (Texture::s_load_queue.find(i) != Texture::s_load_queue.end()) {
					auto current = Texture::s_load_queue.at(i);
					current.tex->m_gl_tex->load(current.data, { current.tex->m_width, current.tex->m_height });
					free(current.data);
					current.tex->m_is_loaded = true;
					Texture::s_load_queue.erase(i);
					std::cout << "texture loaded" << std::endl;
				}
			}
		}
		*/

		if (m_renderer_3d) {
			m_renderer_3d->renderFrame();
		}

		glDisable(GL_DEPTH_TEST);

		if (m_renderer_2d) {
			m_renderer_2d->renderFrame();
		}

		glEnable(GL_DEPTH_TEST);
	}

	void WglContext::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void WglContext::setViewMatrix(Camera& cam) {
		if (m_renderer_3d) {
			m_renderer_3d->setViewMatrix(cam);
		}
	}

	void WglContext::drawMesh(Mesh& mesh, Material& material, Mat4 transform) {
		if (m_renderer_3d) {
			m_renderer_3d->drawMesh(mesh, material, transform);
		}
	}

	void WglContext::loadEnvironmentMap(float* data, iVec2 size) {
		if (m_renderer_3d) {
			m_renderer_3d->loadEnvironmentMap(data, size);
		}
	}

}


#endif