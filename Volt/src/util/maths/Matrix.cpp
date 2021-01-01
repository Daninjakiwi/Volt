#include <util/maths/Matrix.hpp>

namespace volt {
	template Matrix<float, 2>;
	template Matrix<float, 4>;

	Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up) {
		Vec3 f = (center - eye).GetNormalised();
		Vec3 s = f.Cross(up).GetNormalised();
		Vec3 u = s.Cross(f);

		Mat4 out(1.0f);

		out[0] = s.x;
		out[4] = s.y;
		out[8] = s.z;

		out[1] = u.x;
		out[5] = u.y;
		out[9] = u.z;

		out[2] = -f.x;
		out[6] = -f.y;
		out[10] = -f.z;

		out[12] = -(s * eye);
		out[13] = -(u * eye);
		out[14] = f * eye;

		return out;
	}

	Mat4 perspective(float fov, float aspect_ratio, float near_plane, float far_plane) {
		Mat4 out;

		float tan_fov = maths::tand(fov / 2.0f);

		out[0] = 1.0f / (aspect_ratio * tan_fov);
		out[5] = 1.0f / tan_fov;
		out[10] = -(far_plane + near_plane) / (far_plane - near_plane);
		out[11] = -1.0f;
		out[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);

		return out;
	}
}