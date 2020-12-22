#pragma once

namespace volt {
	class GlShader {
	private:
		unsigned int m_id;
	public:
		GlShader(const char* vert_src, const char* frag_src);
		~GlShader();

		unsigned int getId();

		void bind();
	private:
		static unsigned int compile(const char* src, unsigned int type);
	};
}