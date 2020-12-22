#include "render/Quad.hpp"

namespace volt {
	unsigned int count = 0;

	Quad::Quad(Vec2 pos, Vec2 size, Vec4 colour) : id(count), pos(pos),size(size), colour(colour) {
		count++;
	}

	Quad::Quad() : id(count), pos(), size(), colour() {
		count++;
	}
}