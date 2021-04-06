#pragma once
#include <util/maths/Vector.hpp>
#include <util/maths/Matrix.hpp>

namespace volt {
	class Camera {
		friend class GlRenderer3d;
	private:
		Vec3 m_position; //x,y,z
		Vec3 m_rotation; //Pitch, Yaw, Roll (Degrees)
	public:
		Camera();

		void setPosition(Vec3 position);
		void translate(Vec3 translation);

		void setRotation(Vec3 rotation);
		void rotate(Vec3 rotation);

		Mat4 getView();

		operator Mat4();
	private:
		static float wrap(float value); //Keeps rotation between 0-360 degrees, if value too small/large, it wraps aroudn
	};
}