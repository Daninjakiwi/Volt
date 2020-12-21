#pragma once
#include <memory>

#include <util/maths/Maths.hpp>

namespace volt {
	template<typename T, int size>
	class Matrix {
	private:
		T* _data;
		int _size;
	public:
		Matrix() : _data(nullptr), _size(size) {
			_data = (T*)calloc((size_t)maths::square(size), sizeof(T));
		}
		~Matrix() {
			free(_data);
		}

		T& operator[](int index) {
			return _data[index];
		}

		operator T* () const { return _data; }

		Matrix& Transpose() {
			float temp = _data[4];
			_data[4] = _data[1];
			_data[1] = temp;
			return *this;
		}

	};

	typedef Matrix<float, 2> Mat2;
	typedef Matrix<float, 4> Mat4;
}
