/*
Test for the Quick sort algorithm

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#include <random>
#include <cassert>
#include <iostream>
#include <functional>
#include "quick.hh"

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
	auto rand = std::bind(distribution(0, MAX_N), generator(seed));
	int a[MAX_N];
	for (int loop = 0; loop < 1000; ++loop) {
		int size = rand();
		for (int i = 0; i < size; ++i)
			a[i] = rand();
		DSP::quick_sort(a, size);
		for (int i = 1; i < size; ++i)
			assert(a[i-1] <= a[i]);
	}
	std::cerr << "Quick sort algorithm test passed!" << std::endl;
	return 0;
}

