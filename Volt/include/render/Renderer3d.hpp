#pragma once
#include <util/maths/Matrix.hpp>
#include <render/Mesh.hpp>
#include <render/Material.hpp>
#include <render/Camera.hpp>

namespace volt {
	class Renderer3d {
	public:
		Renderer3d() {};
		virtual ~Renderer3d() {};

		virtual void setViewMatrix(Camera& cam) {};
		virtual void drawMesh(Mesh& mesh, Material& material, Mat4 transform) {};
		virtual void loadEnvironmentMap(float* data, iVec2 size) {};

		virtual void renderFrame() {};
	};
}
