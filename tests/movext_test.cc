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

// short runs from the start, with the smallest positive float as an input,
// and negative values otherwise, as with decibels, few of them for many ties
template <int SIZE>
void test_float()
{
	std::random_device ran;
	std::default_random_engine gen(ran());
	std::uniform_int_distribution<int> dis(-3, 0);
	for (int run = 0; run < 10000; ++run) {
		struct Min { float operator () (float a, float b) { return std::min(a, b); } };
		DSP::SWA<float, Min, SIZE> ref_min(std::numeric_limits<float>::max());
		DSP::MovMin<float, SIZE> dut_min;
		struct Max { float operator () (float a, float b) { return std::max(a, b); } };
		DSP::SWA<float, Max, SIZE> ref_max(std::numeric_limits<float>::lowest());
		DSP::MovMax<float, SIZE> dut_max;
		for (int loop = 0; loop < 3 * SIZE; ++loop) {
			int num = dis(gen);
			float val = num ? float(num) : std::numeric_limits<float>::min();
			assert(ref_min(val) == dut_min(val));
			assert(ref_max(val) == dut_max(val));
		}
	}
}

// both at once, against the extrema of the window, with the extreme values and zero,
// which the window starts with, as inputs, from the start and for long
template <int SIZE>
void test_both()
{
	std::random_device ran;
	std::default_random_engine gen(ran());
	const float vals[] = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max(), std::numeric_limits<float>::min(), -2, -1, 0, 1, 2 };
	std::uniform_int_distribution<int> pick(0, 7);
	for (int run = 0; run < 10000; ++run) {
		struct Min { float operator () (float a, float b) { return std::min(a, b); } };
		DSP::SWA<float, Min, SIZE> ref_min(std::numeric_limits<float>::max());
		struct Max { float operator () (float a, float b) { return std::max(a, b); } };
		DSP::SWA<float, Max, SIZE> ref_max(std::numeric_limits<float>::lowest());
		DSP::MovMinMax<float, SIZE> dut;
		for (int loop = 0; loop < 3 * SIZE; ++loop) {
			float val = vals[pick(gen)];
			dut(val);
			assert(ref_min(val) == dut.minimum());
			assert(ref_max(val) == dut.maximum());
		}
	}
	struct Min { int operator () (int a, int b) { return std::min(a, b); } };
	DSP::SWA<int, Min, SIZE> ref_min(std::numeric_limits<int>::max());
	struct Max { int operator () (int a, int b) { return std::max(a, b); } };
	DSP::SWA<int, Max, SIZE> ref_max(std::numeric_limits<int>::min());
	DSP::MovMinMax<int, SIZE> dut;
	std::uniform_int_distribution<int> dis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
	for (int loop = 0; loop < 1000000; ++loop) {
		int val = dis(gen);
		dut(val);
		assert(ref_min(val) == dut.minimum());
		assert(ref_max(val) == dut.maximum());
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
	test_float<1>();
	test_float<2>();
	test_float<3>();
	test_float<4>();
	test_float<16>();
	test_float<42>();
	test_both<1>();
	test_both<2>();
	test_both<3>();
	test_both<4>();
	test_both<16>();
	test_both<42>();
	test_both<123>();

	std::cerr << "Moving extrema test passed!" << std::endl;
	return 0;
}

