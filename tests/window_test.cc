/*
Test for the window functions

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <cmath>
namespace DSP { using std::abs; using std::min; using std::sqrt; using std::cos; using std::sin; }
#include "window.hh"

// every tap is finite, the window is symmetric and both ends are the same, also where rounding
// would put the last tap just outside of the window, as it can with -ffast-math
template <typename TYPE>
bool kaiser_test(TYPE a)
{
	DSP::Kaiser<TYPE> window(a);
	for (int N = 2; N <= 2000; ++N) {
		for (int n = 0; n < N; ++n) {
			TYPE w = window(n, N);
			if (!(w == w) || w < TYPE(0) || w > TYPE(1))
				return false;
			if (w != window(N - 1 - n, N))
				return false;
		}
		if (window(0, N) != window(N - 1, N))
			return false;
	}
	return true;
}

int main()
{
	assert(kaiser_test<float>(2));
	assert(kaiser_test<float>(1));
	assert(kaiser_test<double>(2));
	assert(kaiser_test<double>(3.5));
	std::cerr << "Window functions test passed!" << std::endl;
	return 0;
}
