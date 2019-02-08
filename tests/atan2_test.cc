/*
Test for the atan2 function

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include <limits>
#include <cmath>
#include "unit_circle.hh"
#include "atan2.hh"
#include "const.hh"

template <typename TYPE, typename PRECISE>
void test(int MAX)
{
	for (int N = 1; N <= MAX; ++N) {
		for (int n = 1-N/2; n < N/2; ++n) {
			PRECISE x = DSP::UnitCircle<PRECISE>::cos(n, N);
			PRECISE y = DSP::UnitCircle<PRECISE>::sin(n, N);
			PRECISE a = DSP::atan2<TYPE>(y, x);
			PRECISE b = DSP::Const<PRECISE>::TwoPi() * PRECISE(n) / PRECISE(N);
			PRECISE e = std::abs(a - b);
			assert(e < 10 * std::numeric_limits<TYPE>::epsilon());
		}
	}
}

int main()
{
	test<float, double>(5000);
	test<double, long double>(5000);
	std::cerr << "atan2 function test passed!" << std::endl;
	return 0;
}

