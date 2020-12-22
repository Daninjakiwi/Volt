#pragma once
#include <render/Quad.hpp>

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

		virtual void makeCurrent() {};

		virtual void renderFrame() {};

		virtual void clear() {};
	};
}