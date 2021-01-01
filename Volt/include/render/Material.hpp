#pragma once
#include <render/DynamicTexture.hpp>

namespace volt {
	class Material {
	public:
		virtual Texture& getAlbedo() = 0;
		virtual Texture& getNormal() = 0;
		virtual Texture& getMetallic() = 0;
		virtual Texture& getRoughness() = 0;
		virtual Texture& getAo() = 0;
	};

	class ColourMaterial : public Material {
	private:
		Texture m_tex_colour;
	public:
		ColourMaterial(Vec4 colour);

		Texture& getAlbedo();
		Texture& getNormal();
		Texture& getMetallic();
		Texture& getRoughness();
		Texture& getAo();
	};

	class PbrMaterial : public Material {
	private:
		DynamicTexture m_albedo;
		DynamicTexture m_normal;
		DynamicTexture m_metallic;
		DynamicTexture m_roughness;
		DynamicTexture m_ao;
	public:
		PbrMaterial(const std::string& path);

		Texture& getAlbedo();
		Texture& getNormal();
		Texture& getMetallic();
		Texture& getRoughness();
		Texture& getAo();
	};
}
