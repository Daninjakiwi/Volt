#include <sstream>
#include <fstream>

#include <core/Font.hpp>

#include <core/Resources.hpp>


namespace volt {

	unsigned long long fonts::SEGOE;

	std::unordered_map<unsigned long long, std::unique_ptr<Font>> Font::s_fonts;
	bool Font::s_is_loaded = false;

	void Font::loadFonts() {
		fonts::SEGOE = load("resources/fonts/segoe.fnt", "resources/images/segoe.png");
		s_is_loaded = true;
	}

	Font* Font::get(unsigned long long id) {
		if (!s_is_loaded) {
			loadFonts();
		}
		if (s_fonts.find(id) != s_fonts.end()) {
			return s_fonts[id].get();
		}
		return s_fonts[fonts::SEGOE].get();
	}

	unsigned long long Font::load(const std::string& data_path, const std::string& texture_path) {
		unsigned long long id = resources::assignId();
		s_fonts[id] = std::make_unique<Font>(id, data_path, texture_path);

		return id;
	}

	Font::Font(unsigned long long id, const std::string& data_path, const std::string& texture_path) : m_id(id), m_font_size(0), m_font_height(0), m_line_height(0) , m_tex(texture_path) {

		std::ifstream ss(data_path);
		std::string line;

		std::getline(ss, line);

		m_font_size = std::stoi(line.substr(26, 3));

		std::getline(ss, line);

		m_line_height = std::stoi(line.substr(18, 3));

		m_font_height = std::stoi(line.substr(27, 3));

		std::getline(ss, line);
		std::getline(ss, line);

		int count = std::stoi(line.substr(12, 3));

		for (int i = 0; i < count; i++) {
			std::getline(ss, line);

			unsigned char c = std::stoi(line.substr(8, 3));

			Glyph g;

			g.w = std::stoi(line.substr(36, 3));
			g.h = std::stoi(line.substr(48, 3));
			g.offsetx = std::stoi(line.substr(61, 3));
			g.offsety = std::stoi(line.substr(74, 3));
			g.advance = std::stoi(line.substr(88, 3));

			g.wr = (float)g.w / (float)m_tex.getWidth();
			g.hr = (float)g.h / (float)m_tex.getHeight();

			g.xr = std::stof(line.substr(18, 3)) / (float)m_tex.getWidth();
			g.yr = 1.0f - (std::stof(line.substr(25, 3)) / (float)m_tex.getHeight());
			g.yr -= g.hr;

			m_characters[c] = g;
		}
	}

	float Font::widthOf(const std::string& text, int size) {
		float width = 0;
		float scale = (float)size / (float)m_font_size;

		int count = 0;

		for (unsigned char c : text) {
			count++;
			if (count == text.length()) {
				width += m_characters[c].w * scale;
			}
			else {
				width += m_characters[c].advance * scale;
			}
		}
		return width;
	}

	unsigned long long Font::getId() {
		return m_id;
	}
}