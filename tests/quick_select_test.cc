/*
Test for the Quick select algorithm

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#include <random>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <functional>
#include "quick_select.hh"

int main()
{
	const int MAX_N = 1 << 16;
	unsigned seed = 42;
	if (1) {
		std::random_device rd;
		seed = rd();
	}
	typedef std::default_random_engine generator;
	typedef std::uniform_int_distribution<int> distribution;
	auto rand = std::bind(distribution(1, MAX_N), generator(seed));
	int a[MAX_N], b[MAX_N];
	for (int loop = 0; loop < 10000; ++loop) {
		int size = rand();
		for (int i = 0; i < size; ++i)
			a[i] = b[i] = rand();
		int k = rand() % size;
		std::nth_element(a, a+k, a+size);
		int v = DSP::quick_select(b, k, size);
		assert(v == a[k]);
	}
	std::cerr << "Quick select algorithm test passed!" << std::endl;
	return 0;
}

