#pragma once
#include <util/maths/Vector.hpp>

namespace volt {
	enum class ContextType {
		OPENGL, VULKAN, DIRECTX9, DIRECTX11, DIRECTX12
	};

	class GraphicsContext {
	public:
		GraphicsContext() {};
		virtual ~GraphicsContext() {};

		virtual void setBackgroundColour(Vec4 colour) {};

		virtual void drawQuad2D(Vec2 pos, Vec2 size, Vec4 colour) {};

		virtual void makeCurrent() {};

		virtual void clear() {};
	};
}