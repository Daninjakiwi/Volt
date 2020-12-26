#pragma once

#include <unordered_map>

#include <render/opengl/GlShader.hpp>
#include <render/Renderer2d.hpp>

#include <render/Texture.hpp>

namespace volt {
	class Command {
	public:
		Command() {};
		virtual ~Command() {};
		virtual void execute() {};
	};

	class DrawCommand : public Command {
	private:
		unsigned long long m_start;
		unsigned long long m_count;
	public:
		DrawCommand(unsigned long long start, unsigned long long count);

		void execute();
	};

	class BindTextureCommand : public Command {
	private:
		unsigned int m_id;
		unsigned int m_slot;
	public:
		BindTextureCommand(unsigned int id, unsigned int slot);
		
		void execute();
	};

	//FLAGS
	//NO UPDATE - Draw this quad but dont update data in buffer
	//SEPERATE - If quad already exists, create a new one in the buffer
	//DELETE - After drawing the quad, delete from the buffer


	class GlRenderer2d : public Renderer2d {
	private:
		GlShader* m_shader;
		float* m_projection;
		unsigned int m_vert_array, m_vert_buffer, m_elem_buffer;

		std::unordered_map<unsigned long long, std::vector<unsigned long long>> m_locations_map; //maps resource id -> vector of buffer indexes containing that resource
		std::unordered_map<unsigned long long, unsigned int> m_texture_map; //maps resource id -> texture unit (cleared every frame)
		std::unordered_map<unsigned int, unsigned long long> m_texture_map_invert; //maps texture unit -> resource id
		unsigned long long m_count, m_delete_count, m_quad_count, m_first,m_last;
		bool m_is_empty;
		std::vector<Command*> m_command_queue; 

	public:
		GlRenderer2d(iVec2 size);
		~GlRenderer2d();

		void drawQuad(Quad& quad, unsigned int flags);

		void drawTexture(Texture& tex, Vec2 pos, Vec2 size);
		void drawTexture(Texture& tex, Vec2 pos, Vec2 size, Vec2* coords);
		void drawTexture(Texture& tex, Vec2 pos, Vec2 size, Vec2* coords, Vec4 colour_filter);

		void drawString(const std::string& text, Vec2 pos, unsigned int size, unsigned long long font, Vec4 colour);
		void drawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, Vec4 colour);

		void renderFrame();
	private:
		void insertIndices(unsigned long long index, unsigned int* indices);
		void insertVertices(unsigned long long index, float* vertices);
	};
}
