/*
Test for the discrete Hilbert transformation

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <cmath>
namespace DSP { using std::min; using std::abs; using std::sqrt; }
#include "complex.hh"
#include "hilbert.hh"

// compare to the convolution with all taps, the zero ones included, delayed by one sample
template <typename TYPE, int TAPS>
bool convolution_test()
{
	typedef DSP::Complex<TYPE> cmplx;
	DSP::Kaiser<double> window(2);
	double kernel[TAPS];
	for (int n = 0; n < TAPS; ++n) {
		int x = n - (TAPS - 1) / 2;
		kernel[n] = x & 1 ? window(n, TAPS) * 2 / (DSP::Const<double>::Pi() * x) : 0;
	}
	DSP::Hilbert<cmplx, TAPS> hilbert;
	std::minstd_rand rand(TAPS);
	std::uniform_real_distribution<double> dist(-1, 1);
	const int LEN = 4 * TAPS;
	double input[LEN];
	for (int n = 0; n < LEN; ++n) {
		input[n] = dist(rand);
		cmplx output = hilbert(input[n]);
		double re = 0, im = 0;
		int mid = n - 1 - (TAPS - 1) / 2;
		if (mid >= 0)
			re = window((TAPS - 1) / 2, TAPS) * input[mid];
		for (int k = 0; k < TAPS && k <= n - 1; ++k)
			im += kernel[k] * input[n-1-k];
		if (std::abs(output.real() - re) > 0.0001 || std::abs(output.imag() - im) > 0.0001)
			return false;
	}
	return true;
}

int main()
{
	assert((convolution_test<float, 5>()));
	assert((convolution_test<float, 9>()));
	assert((convolution_test<float, 33>()));
	assert((convolution_test<float, 481>()));
	assert((convolution_test<double, 129>()));
	std::cerr << "Hilbert transformation test passed!" << std::endl;
	return 0;
}
