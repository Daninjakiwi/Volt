#pragma once
#include <util/maths/Vector.hpp>

namespace volt {
	struct Quad {
		unsigned int id;
		Vec2 pos;
		Vec2 size;
		Vec4 colour;

		Quad(Vec2 pos, Vec2 size, Vec4 colour);
		Quad();
	};
}
