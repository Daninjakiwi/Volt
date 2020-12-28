#include <core/Resources.hpp>

namespace volt::resources {
	unsigned long long s_current = 1;
	
	unsigned long long assignId() {
		s_current++;
		return s_current;
	}
}