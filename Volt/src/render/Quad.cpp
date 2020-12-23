#include <render/Quad.hpp>

#include <core/Resources.hpp>

namespace volt {
	Quad::Quad(Vec2 pos, Vec2 size, Vec4 colour) : id(resources::assignId()), pos(pos),size(size), colour(colour) {
	}

	Quad::Quad() : id(resources::assignId()), pos(), size(), colour() {
	}
}