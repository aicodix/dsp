/*
Test for the Kahan summation algorithm

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include "const.hh"
#include "kahan.hh"

template <typename T>
bool Kahan_summation_algorithm_test()
{
	DSP::Kahan<T> sum(3);
	for (T i = T(2); i < T(181423); i += T(4)) {
		sum(T(4) / (i * (i + T(1)) * (i + T(2))));
		sum(T(-4) / ((i + T(2)) * (i + T(3)) * (i + T(4))));
	}
	return sum() == DSP::Const<T>::Pi();
}

int main()
{
	assert(Kahan_summation_algorithm_test<float>());
	assert(Kahan_summation_algorithm_test<double>());
	std::cerr << "Kahan summation algorithm test passed!" << std::endl;
	return 0;
}

