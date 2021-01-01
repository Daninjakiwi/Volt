#pragma once
#include <memory>

#include <util/maths/Maths.hpp>
#include <util/maths/Vector.hpp>

namespace volt {
	template<typename T, int size>
	class Matrix {
	private:
		T _data[size * size];
		int _size;
	public:
		Matrix() : _size(size) {
			init();
		}

		Matrix(T value) : _size(size) {
			init();
			for (int i = 0; i < size; i++) {
				_data[(i * size) + i] = value;
			}
		}

		~Matrix() {
		}

		void translate(Vector3<T> vec) {
			_data[12] += vec.x;
			_data[13] += vec.y;
			_data[14] += vec.z;
		}

		void rotate(T degrees, Vector3<T> axis) {
			T const c = maths::cosd(degrees);
			T const s = maths::sind(degrees);

			Vector3<T> temp(axis * (T(1) - c));

			Matrix<T,4> rotate;
			rotate[0] = c + temp.x * axis.x;
			rotate[1] = temp.x * axis.y + s * axis.z;
			rotate[2] = temp.x * axis.z - s * axis.y;

			rotate[4] = temp.y * axis.x - s * axis.z;
			rotate[5] = c + temp.y * axis.y;
			rotate[6] = temp.y * axis.z + s * axis.x;

			rotate[8] = temp.z * axis.x + s * axis.y;
			rotate[9] = temp.z * axis.y - s * axis.x;
			rotate[10] = c + temp.z * axis.z;

			Vector3<T> first(_data[0], _data[1], _data[2]);
			Vector3<T> second(_data[4], _data[5], _data[6]);
			Vector3<T> third(_data[8], _data[9], _data[10]);

			Vector3<T> out0 = first * rotate[0] + second * rotate[1] + third * rotate[2];
			Vector3<T> out1 = first * rotate[4] + second * rotate[5] + third * rotate[6];
			Vector3<T> out2 = first * rotate[8] + second * rotate[9] + third * rotate[10];

			_data[0] = out0.x;
			_data[1] = out0.y;
			_data[2] = out0.z;

			_data[4] = out1.x;
			_data[5] = out1.y;
			_data[6] = out1.z;

			_data[8] = out2.x;
			_data[9] = out2.y;
			_data[10] = out2.z;
		}

		void scale(Vector3<T> scale) {
			_data[0] *= scale.x;
			_data[1] *= scale.x;
			_data[2] *= scale.x;

			_data[4] *= scale.y;
			_data[5] *= scale.y;
			_data[6] *= scale.y;

			_data[8] *= scale.z;
			_data[9] *= scale.z;
			_data[10] *= scale.z;
		}

		T& operator[](int index) {
			return _data[index];
		}
	private:
		void init() {
			for (int i = 0; i < size * size; i++) {
				_data[i] = 0.0f;
			}
		}
	};

	typedef Matrix<float, 2> Mat2;
	typedef Matrix<float, 4> Mat4;

	Mat4 lookAt(Vec3 eye, Vec3 center, Vec3 up);
	Mat4 perspective(float fov, float aspect_ratio, float near_plane, float far_plane);
}