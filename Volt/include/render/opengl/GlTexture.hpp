#pragma once

#include <util/maths/Vector.hpp>

namespace volt {
	class GlTexture {
	private:
		unsigned int m_id;
	public:
		GlTexture(void* data, iVec2 size);
		GlTexture();
		~GlTexture();

		void load(void* data, iVec2 size);

		unsigned int getId() const;
	};
}
