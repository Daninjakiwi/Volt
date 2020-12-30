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
}