#include <math.h>
constexpr double PI = 3.14159265358979323846;

namespace volt::maths {

	double square(double x) {
		return x * x;
	}

	int factorial(int x) {
		if (x <= 1) {
			return 1;
		}
		return x * factorial(x - 1);
	}

	double mod(double a, double b) {
		return fmod(a, b);
	}

	double squareroot(double x) {
		return sqrt(x);
	}

	double max(double a, double b) {
		return a > b ? a : b;
	}

	double min(double a, double b) {
		return a < b ? a : b;
	}

	double clamp(double value, double _min, double _max) {
		return max(_min, min(_max, value));
	}

	double cosd(double degrees) {
		return cos(degrees * PI / 180.0);
	}

	double sind(double degrees) {
		return sin(degrees * PI / 180.0);
	}

	double tand(double degrees) {
		return tan(degrees * PI / 180.0);
	}

}