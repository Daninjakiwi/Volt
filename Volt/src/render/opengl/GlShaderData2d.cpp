#pragma once

namespace volt {
	const char* vert_2d = R"(
		#version 330 core

		layout (location=0) in vec2 position;
		layout (location=1) in vec2 tex_coord;
		layout (location=2) in vec4 colour;
		layout (location=3) in float tex_index;

		out vec2 v_tex_coord;
		out vec4 v_colour;
		out float v_tex_index;

		uniform mat4 u_projection;

		void main() {
			v_tex_coord = tex_coord;
			v_colour = colour;
			v_tex_index = tex_index;

			gl_Position = u_projection * vec4(position, 0.0,1.0);
		}
	)";

	const char* frag_2d = R"(
		#version 330 core
		
		layout (location=0) out vec4 col;
		
		in vec2 v_tex_coord;
		in vec4 v_colour;
		in float v_tex_index;

		uniform sampler2D u_textures[16];

		void main() {
			int index = int(v_tex_index);
			
			if (index > 0) {
				vec4 tex = texture(u_textures[index-1],v_tex_coord);
				float alpha = tex.a;
				if (v_colour.a > 0) {
					col = vec4(v_colour.r,v_colour.g,v_colour.b,v_colour.a * alpha);
				} else {
					col = tex;
				}

				
			} else {
				col = v_colour;
			}
		}		
	)";

}
