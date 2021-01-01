#pragma once
#include <util/maths/Matrix.hpp>
#include <render/Mesh.hpp>
#include <render/Material.hpp>

namespace volt {
	class Renderer3d {
	public:
		Renderer3d() {};
		virtual ~Renderer3d() {};

		virtual void setViewMatrix(Mat4 matrix) {};
		virtual void drawMesh(Mesh& mesh, Material& material, Mat4 transform) {};
		virtual void loadEnvironmentMap(float* data, iVec2 size) {};

		virtual void renderFrame() {};
	};
}
