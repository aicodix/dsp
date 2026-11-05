/*
Test for the sin and cos functions

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include <limits>
#include <algorithm>
#include <cmath>
namespace DSP { using std::nearbyint; }
#include "sincos.hh"

template <typename TYPE, typename PRECISE>
void test(TYPE range, int steps)
{
	for (int i = -steps; i <= steps; ++i) {
		TYPE x = range * TYPE(i) / TYPE(steps);
		PRECISE s = std::sin(PRECISE(x));
		PRECISE c = std::cos(PRECISE(x));
		PRECISE es = std::abs(PRECISE(DSP::sin<TYPE>(x)) - s);
		PRECISE ec = std::abs(PRECISE(DSP::cos<TYPE>(x)) - c);
		// the argument reduction loses what the argument itself can't resolve
		PRECISE limit = 2 * std::numeric_limits<TYPE>::epsilon() * std::max(PRECISE(1), std::abs(PRECISE(x)));
		assert(es < limit);
		assert(ec < limit);
	}
}

int main()
{
	test<float, double>(1, 1000000);
	test<float, double>(10000, 1000000);
	test<double, long double>(1, 1000000);
	test<double, long double>(10000, 1000000);
	std::cerr << "sin and cos function test passed!" << std::endl;
	return 0;
}

