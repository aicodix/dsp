/*
Test for the CORDIC atan2 function

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include <cmath>
#include "unit_circle.hh"
#include "cordic.hh"
#include "const.hh"

template <typename TYPE>
void test()
{
	const int N = 1 << (8*sizeof(TYPE));
	for (int n = -N/2; n < N/2; ++n) {
		TYPE x = nearbyint((N/2-1) * DSP::UnitCircle<double>::cos(n, N));
		TYPE y = nearbyint((N/2-1) * DSP::UnitCircle<double>::sin(n, N));
		TYPE a = DSP::CORDIC<TYPE>::atan2(y, x);
		assert(abs(a-n) <= 1);
	}
}

int main()
{
	test<char>();
	test<short>();
	std::cerr << "CORDIC atan2 test passed!" << std::endl;
	return 0;
}

