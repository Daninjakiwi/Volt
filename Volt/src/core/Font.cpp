#include <sstream>
#include <fstream>

#include <core/Font.hpp>

#include <core/Resources.hpp>


namespace volt {

	unsigned long long fonts::SEGOE = 1;

	std::unordered_map<unsigned long long, std::unique_ptr<Font>> Font::s_fonts;
	bool Font::s_is_loaded = false;

	void Font::loadFonts() {
		s_fonts[1] = std::make_unique<Font>(1, "resources/fonts/segoe.fnt", "resources/images/segoe.png");
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

		auto start = line.find("size=") + 5;
		auto end = line.find(" ", start);

		m_font_size = std::stoi(line.substr(start, end - start));

		std::getline(ss, line);

		start = line.find("lineHeight=") + 11;
		end = line.find(" ", start);

		m_line_height = std::stoi(line.substr(start, end - start));


		start = line.find("base=") + 5;
		end = line.find(" ", start);

		m_font_height = std::stoi(line.substr(start, end - start));

		start = line.find("scaleW=") + 7;
		end = line.find(" ", start);

		float img_width = std::stof(line.substr(start, end - start));

		start = line.find("scaleH=") + 7;
		end = line.find(" ", start);

		float img_height = std::stof(line.substr(start, end - start));


		std::getline(ss, line);
		std::getline(ss, line);

		start = line.find("count=") + 6;
		end = line.find(" ", start);

		int count = std::stoi(line.substr(start, end - start));

		for (int i = 0; i < count; i++) {

			std::getline(ss, line);

			start = line.find("id=") + 3;
			end = line.find(" ", start);

			unsigned char c = std::stoi(line.substr(start, end - start));

			Glyph g;

			start = line.find("width=") + 6;
			end = line.find(" ", start);

			g.w = std::stoi(line.substr(start, end - start));

			start = line.find("height=") + 7;
			end = line.find(" ", start);

			g.h = std::stoi(line.substr(start, end - start));

			start = line.find("xoffset=") + 8;
			end = line.find(" ", start);

			g.offsetx = std::stoi(line.substr(start, end - start));

			start = line.find("yoffset=") + 8;
			end = line.find(" ", start);

			g.offsety = std::stoi(line.substr(start, end - start));

			start = line.find("xadvance=") + 9;
			end = line.find(" ", start);

			g.advance = std::stoi(line.substr(start, end - start));

			g.wr = (float)g.w / img_width;
			g.hr = (float)g.h / img_height;

			start = line.find("x=") + 2;
			end = line.find(" ", start);

			g.xr = std::stof(line.substr(start,end-start)) / img_width;

			start = line.find("y=") + 2;
			end = line.find(" ", start);

			g.yr = 1.0f - (std::stof(line.substr(start, end - start)) / img_height);
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