#pragma once

#include <util/maths/Vector.hpp>

namespace volt {
	class GlTexture {
	private:
		unsigned int m_id;
	public:
		GlTexture(void* data, iVec2 size);
		~GlTexture();

		unsigned int getId() const;
	};
}
