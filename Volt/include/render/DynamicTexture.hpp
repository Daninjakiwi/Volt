#pragma once

#include <string>

#include <render/Texture.hpp>

namespace volt {
	enum class Quality {
		ULTRA = 0, HIGH = 1, MEDIUM = 2, LOW = 3
	};

	class DynamicTexture {
	private:
		static Quality s_current;
		static Texture s_default;

		std::string m_path;
		Texture m_textures[4];
	public:
		static void setQuality(Quality quality);

		DynamicTexture(const std::string& path);

		operator Texture& ();
	};
}
