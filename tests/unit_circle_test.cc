/*
Test for the UnitCircle functions

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include <limits>
#include <cmath>
#include "unit_circle.hh"
#include "const.hh"

template <typename TYPE, typename PRECISE>
void test(int MAX)
{
	for (int N = 1; N <= MAX; ++N) {
		for (int n = 0; n < N; ++n) {
			PRECISE x = DSP::Const<PRECISE>::TwoPi() * PRECISE(n) / PRECISE(N);
			PRECISE cos_err = std::abs(DSP::UnitCircle<TYPE>::cos(n, N) - std::cos(x));
			assert(cos_err < std::numeric_limits<TYPE>::epsilon());
			PRECISE sin_err = std::abs(DSP::UnitCircle<TYPE>::sin(n, N) - std::sin(x));
			assert(sin_err < std::numeric_limits<TYPE>::epsilon());
		}
	}
}

int main()
{
	test<float, double>(5000);
	test<double, long double>(5000);
	std::cerr << "UnitCircle functions test passed!" << std::endl;
	return 0;
}

