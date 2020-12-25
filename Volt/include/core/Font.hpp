#pragma once
#include <unordered_map>
#include <memory>

#include <render/Texture.hpp>

namespace volt {
	namespace fonts {
		extern unsigned long long SEGOE;
	}


	struct Glyph {
		float xr, yr, wr, hr, w, h, offsetx, offsety, advance;
	};

	class Font {
		friend class GlRenderer2d;
	private:
		static std::unordered_map<unsigned long long, std::unique_ptr<Font>> s_fonts;

		unsigned long long m_id;

		int m_font_size, m_font_height, m_line_height;
		Texture m_tex;

		std::unordered_map<unsigned char, Glyph> m_characters;

	public:
		static unsigned long long load(const std::string& data_path, const std::string& texture_path);
		static Font* get(unsigned long long id);

		Font(unsigned long long id, const std::string& data_path, const std::string& texture_path);

		float widthOf(const std::string& text, int size);

		unsigned long long getId();
	private:
		
		static void loadFonts();
		static bool s_is_loaded;
	};
}
