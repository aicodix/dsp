/*
Test for Moving Extrema

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <cassert>
#include <random>
#include <limits>
#include "swa.hh"
#include "movext.hh"

template <int SIZE>
void test()
{
	struct Min { int operator () (int a, int b) { return std::min(a, b); } };
	DSP::SWA<int, Min, SIZE> ref_min(std::numeric_limits<int>::max());
	DSP::MovMin<int, SIZE> dut_min;
	struct Max { int operator () (int a, int b) { return std::max(a, b); } };
	DSP::SWA<int, Max, SIZE> ref_max(std::numeric_limits<int>::min());
	DSP::MovMax<int, SIZE> dut_max;
	std::random_device ran;
	std::default_random_engine gen(ran());
	std::uniform_int_distribution<int> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
	for (int loop = 0; loop < 1000000; ++loop) {
		int val = dis(gen);
		assert(ref_min(val) == dut_min(val));
		assert(ref_max(val) == dut_max(val));
	}
}

int main()
{
	test<1>();
	test<2>();
	test<3>();
	test<4>();
	test<5>();
	test<6>();
	test<7>();
	test<8>();
	test<9>();
	test<10>();
	test<12>();
	test<13>();
	test<14>();
	test<15>();
	test<16>();
	test<42>();
	test<123>();
	test<4567>();

	std::cerr << "Moving extrema test passed!" << std::endl;
	return 0;
}

