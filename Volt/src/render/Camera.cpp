#include <render/Camera.hpp>

namespace volt {
	Camera::Camera() : m_position(0.0f,0.0f,0.0f), m_rotation(0.0f,-90.0f, 0.0f) {

	}

	void Camera::setPosition(Vec3 position) {
		m_position = position;
	}

	void Camera::translate(Vec3 translation) {
		m_position += translation;
	}

	void Camera::setRotation(Vec3 rotation) {
		m_rotation = rotation;
	}

	void Camera::rotate(Vec3 rotation) {
		m_rotation.x = maths::clamp((double)m_rotation.x + rotation.x, -89.0, 89.0);
		m_rotation.y = m_rotation.y + rotation.y;	
		m_rotation.z = m_rotation.z + rotation.z;
	}

	Mat4 Camera::getView() {	
		Vec3 front;
		front.x = maths::cosd(m_rotation.y) * maths::cosd(m_rotation.x);
		front.y = maths::sind(m_rotation.x);
		front.z = maths::sind(m_rotation.y) * maths::cosd(m_rotation.x);
		
		front = front.GetNormalised();

		Vec3 center = m_position + front;
		Vec3 up(0.0f, 1.0f, 0.0f);

		Vec3 f = (center - m_position).GetNormalised();
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

		out[12] = -(s * m_position);
		out[13] = -(u * m_position);
		out[14] = f * m_position;
	
		return out;
	}

	float Camera::wrap(float value) {
		if (value > 360.0f) {
			value = maths::mod(value, 360.0f);
		}
		else if (value < 0.0f) {
			value = 360.0f - maths::mod(value, 360.0f);
		}
		return value;
	}

	Camera::operator Mat4() {
		return getView();
	}
}