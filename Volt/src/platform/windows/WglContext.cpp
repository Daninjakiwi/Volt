#ifdef BUILD_WINDOWS
#include <platform/windows/WglContext.hpp>
#include <render/opengl/GlFunctions.hpp>

namespace volt {
	WglContext::WglContext(Window* window) : m_device(nullptr), m_context(nullptr), m_renderer_2d(nullptr) {
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
			//wglSwapIntervalEXT(1);

			m_context = wglCreateContextAttribsARB(m_device, 0, attribs);

			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(temp_context);

			wglMakeCurrent(m_device, m_context);

			loadGlFunctions();

			glViewport(0, 0, window->m_size.x, window->m_size.y);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			if (major == 4) {
				m_renderer_2d = new GlRenderer2d(window->m_size);
			}
			else if (major == 3) {
				m_renderer_2d = new GlRenderer2d(window->m_size);
			}
		}

	}

	WglContext::~WglContext() {

	}

	void WglContext::setBackgroundColour(Vec4 colour) {
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void WglContext::drawQuad(Quad& quad, unsigned int flags) {
		if (m_renderer_2d) {
			m_renderer_2d->drawQuad(quad, flags);
		}
	}

	void WglContext::makeCurrent() {
		wglMakeCurrent(m_device, m_context);
	}

	void WglContext::renderFrame() {
		if (m_renderer_2d) {
			m_renderer_2d->renderFrame();
		}
	}

	void WglContext::clear() {
		glClear(GL_COLOR_BUFFER_BIT);
	}
}


#endif