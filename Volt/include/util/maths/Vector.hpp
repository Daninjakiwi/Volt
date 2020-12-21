#pragma once
#include <iostream>

#include <util/maths/Maths.hpp>

namespace volt {
	template<typename T>
	class Vector2 {
	public:
		union {
			T x;
			T r;
			T width;
		};
		union {
			T y;
			T g;
			T height;
		};
	public:
		Vector2() : x((T)0), y((T)0) {
		}
		explicit Vector2(T n) : x(n), y(n) {
		}
		Vector2(T x, T y) : x(x), y(y) {
		}

		T Length() {
			return (T)maths::squareroot(maths::square(x) + maths::square(y));
		}

		Vector2& Normalise() {
			T len = Length();
			x /= len;
			y /= len;
			return *this;
		}

		Vector2 GetNormalised() {
			return Vector2(*this).Normalise();
		}

		//Vector equality
		bool operator==(const Vector2& vec) {
			return x == vec.x && y == vec.y;
		}

		bool operator!=(const Vector2& vec) {
			return !(*this == vec);
		}

		//Assign vector
		Vector2& operator=(const Vector2& vec) {
			x = vec.x;
			y = vec.y;
			return *this;
		}

		//Add two vectors
		Vector2 operator+(const Vector2& vec) {
			return Vector2(*this) += vec;
		}

		Vector2& operator+=(const Vector2& vec) {
			x += vec.x;
			y += vec.y;
			return *this;
		}

		//Subtract two vectors
		Vector2 operator-(const Vector2& vec) {
			return Vector2(*this) -= vec;
		}

		Vector2& operator-=(const Vector2& vec) {
			x -= vec.x;
			y -= vec.y;
			return *this;
		}

		//Divide two vectors
		Vector2 operator/(const Vector2& vec) {
			return Vector2(*this) /= vec;
		}

		Vector2& operator/=(const Vector2& vec) {
			x /= vec.x;
			y /= vec.y;
			return *this;
		}

		Vector2 operator/(const T scalar) {
			return Vector2(*this) /= scalar;
		}

		Vector2& operator/=(const T scalar) {
			x /= scalar;
			y /= scalar;
			return *this;
		}

		//Multiply by a scalar value
		Vector2 operator*(const T scalar) {
			return Vector2(*this) *= scalar;
		}

		Vector2& operator*=(const T scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}

		//Dot product of two Vectors
		T operator*(const Vector2& vec) {
			return (x * vec.x) + (y * vec.y);
		}
	};

	template<typename T>
	class Vector3 : public Vector2<T> {
	public:
		union {
			T z;
			T b;
		};
	public:
		Vector3() : Vector2<T>(), z((T)0) {
		}
		explicit Vector3(T n) : Vector2<T>(n), z(n) {
		}
		Vector3(T x, T y, T z) : Vector2<T>(x, y), z(z) {
		}

		//Convert from vector2 to vector3
		Vector3(const Vector2<T>& vec) : Vector2<T>(vec), z((T)0) {

		}

		T Length() {
			return (T)maths::squareroot(maths::square(this->x) + maths::square(this->y) + maths::square(z));
		}

		//Normalise
		Vector3& Normalise() {
			T len = Length();
			this->x /= len;
			this->y /= len;
			z /= len;
			return *this;
		}

		Vector3 GetNormalised() {
			return Vector3(*this).Normalise();
		}

		//Vector equality
		bool operator==(const Vector3& vec) {
			return this->x == vec.x && this->y == vec.y && z == vec.z;
		}

		bool operator!=(const Vector3& vec) {
			return !(*this == vec);
		}

		//Assign vector
		Vector3& operator=(const Vector3& vec) {
			this->x = vec.x;
			this->y = vec.y;
			z = vec.z;
			return *this;
		}

		//Add two vectors
		Vector3 operator+(const Vector3& vec) {
			return Vector3(*this) += vec;
		}

		Vector3& operator+=(const Vector3& vec) {
			this->x += vec.x;
			this->y += vec.y;
			z += vec.z;
			return *this;
		}

		//Subtract two vectors
		Vector3 operator-(const Vector3& vec) {
			return Vector3(*this) -= vec;
		}

		Vector3& operator-=(const Vector3& vec) {
			this->x -= vec.x;
			this->y -= vec.y;
			z -= vec.z;
			return *this;
		}

		//Divide two vectors
		Vector3 operator/(const Vector3& vec) {
			return Vector3(*this) /= vec;
		}

		Vector3& operator/=(const Vector3& vec) {
			this->x /= vec.x;
			this->y /= vec.y;
			z /= vec.z;
			return *this;
		}

		Vector3 operator/(const T scalar) {
			return Vector3(*this) /= scalar;
		}

		Vector3& operator/=(const T scalar) {
			this->x /= scalar;
			this->y /= scalar;
			z /= scalar;
			return *this;
		}

		//Multiply by a scalar value
		Vector3 operator*(const T scalar) {
			return Vector3(*this) *= scalar;
		}

		Vector3& operator*=(const T scalar) {
			this->x *= scalar;
			this->y *= scalar;
			z *= scalar;
			return *this;
		}

		//Dot product
		T operator*(const Vector3& vec) {
			return (this->x * vec.x) + (this->y * vec.y) + (z * vec.z);
		}

		//Cross product
		Vector3 Cross(const Vector3& vec) {
			T x = (this->y * vec.z) - (z * vec.y);
			T y = (z * vec.x) - (this->x * vec.z);
			T z = (this->x * vec.y) - (this->y * vec.x);
			return Vector3(x, y, z);
		}

	};

	template<typename T>
	class Vector4 : public Vector3<T> {
	public:
		union {
			T w;
			T a;
		};
	public:
		Vector4() : Vector3<T>(), w((T)0) {
		}
		explicit Vector4(T n) : Vector3<T>(n), w(n) {
		}
		Vector4(T x, T y, T z, T w) : Vector3<T>(x, y, z), w(w) {
		}

		Vector4 operator/(const T scalar) {
			return Vector4(*this) /= scalar;
		}

		Vector4& operator/=(const T scalar) {
			this->x /= scalar;
			this->y /= scalar;
			this->z /= scalar;
			w /= scalar;
			return *this;
		}

		//Add two vectors
		Vector4 operator+(const Vector4& vec) {
			return Vector4(*this) += vec;
		}

		Vector4& operator+=(const Vector4& vec) {
			this->x += vec.x;
			this->y += vec.y;
			this->z += vec.z;
			w += vec.w;
			return *this;
		}
	};

	typedef Vector2<float> Vec2;
	typedef Vector3<float> Vec3;
	typedef Vector4<float> Vec4;

	typedef Vector2<int> iVec2;
	typedef Vector3<int> iVec3;
	typedef Vector4<int> iVec4;

	typedef Vector2<unsigned int> uVec2;
	typedef Vector3<unsigned int> uVec3;
	typedef Vector4<unsigned int> uVec4;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const volt::Vector2<T>& vec)
{
	os << vec.x << ", " << vec.y;
	return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const volt::Vector3<T>& vec)
{
	os << vec.x << ", " << vec.y << ", " << vec.z;
	return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const volt::Vector4<T>& vec)
{
	os << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
	return os;
}