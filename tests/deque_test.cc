/*
Test for the double-ended queue

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <cassert>
#include <random>
#include <deque>
#include "deque.hh"

template <int SIZE>
void test()
{
	std::deque<int> ref;
	DSP::Deque<int, SIZE> dut;
	std::random_device ran;
	std::default_random_engine gen(ran());
	typedef std::uniform_int_distribution<int> dis;
	for (int loop = 0; loop < 1000000; ++loop) {
		assert((int)ref.size() == dut.size());
		int operation = dis(0, 5)(gen);
		switch (operation) {
			case 0:
				assert(ref.empty() == dut.empty());
				if (!dut.empty())
					assert(ref.front() == dut.front());
				break;
			case 1:
				assert(ref.empty() == dut.empty());
				if (!dut.empty())
					assert(ref.back() == dut.back());
				break;
			case 2:
				for (int n = dis(0, dut.size())(gen), i = 0; i < n; ++i) {
					ref.pop_front();
					dut.pop_front();
				}
				break;
			case 3:
				for (int n = dis(0, dut.size())(gen), i = 0; i < n; ++i) {
					ref.pop_back();
					dut.pop_back();
				}
				break;
			case 4:
				for (int n = dis(0, dut.max_size() - dut.size())(gen), i = 0; i < n; ++i) {
					int val = gen();
					ref.push_front(val);
					dut.push_front(val);
				}
				break;
			case 5:
				for (int n = dis(0, dut.max_size() - dut.size())(gen), i = 0; i < n; ++i) {
					int val = gen();
					ref.push_back(val);
					dut.push_back(val);
				}
				break;
		}
	}
}

int main()
{
	DSP::Deque<int, 1> dut;
	assert(dut.empty());
	assert(!dut.full());
	assert(dut.size() == 0);
	assert(dut.max_size() == 1);
	dut.push_front(42);
	assert(dut.full());
	assert(dut.size() == 1);
	assert(dut.front() == 42);
	assert(dut.back() == 42);
	dut.pop_back();
	assert(dut.empty());
	dut.push_back(42);
	assert(dut.full());
	assert(dut.size() == 1);
	assert(dut.front() == 42);
	assert(dut.back() == 42);
	dut.pop_front();
	test<1>();
	test<2>();
	test<3>();
	test<4>();
	test<5>();
	test<6>();
	test<7>();
	test<8>();
	test<42>();
	test<123>();
	std::cerr << "Double-ended queue test passed!" << std::endl;
	return 0;
}

