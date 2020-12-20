#include <math.h>

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
}