/*
Test for the finite impulse response filter

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <cmath>
namespace DSP { using std::min; using std::abs; using std::sin; using std::cos; using std::sqrt; }
#include "complex.hh"
#include "filter.hh"
#include "window.hh"
#include "fir.hh"

template <int TAPS>
bool integer_convolution_test()
{
	std::minstd_rand rand(TAPS);
	std::uniform_int_distribution<int> dist(-1000, 1000);
	int kernel[TAPS];
	for (int i = 0; i < TAPS; ++i)
		kernel[i] = dist(rand);
	DSP::FIR<int, int, TAPS> fir(kernel);
	const int LEN = 4 * TAPS;
	int input[LEN];
	for (int n = 0; n < LEN; ++n) {
		input[n] = dist(rand);
		int expected = 0;
		for (int k = 0; k < TAPS && k <= n; ++k)
			expected += kernel[k] * input[n-k];
		if (fir(input[n]) != expected)
			return false;
	}
	return true;
}

template <int TAPS>
bool impulse_response_test()
{
	DSP::FIR<float, float, TAPS> fir(DSP::LowPass<float>(0.1f), DSP::Kaiser<float>(2));
	// float taps differ from the double reference, and between compilers, by
	// rounding, which is large relative to taps near zero, so compare to the peak
	DSP::LowPass<double> kernel(0.1);
	DSP::Kaiser<double> window(2);
	double peak = 0;
	for (int n = 0; n < TAPS; ++n)
		peak = std::max(peak, std::abs(kernel(n, TAPS) * window(n, TAPS)));
	for (int n = 0; n < 2 * TAPS; ++n) {
		float output = fir(n ? 0 : 1);
		if (n >= TAPS) {
			if (output != 0)
				return false;
			continue;
		}
		double expected = kernel(n, TAPS) * window(n, TAPS);
		if (std::abs(output - expected) > 0.0001 * peak)
			return false;
	}
	return true;
}

template <int TAPS>
bool complex_convolution_test()
{
	typedef DSP::Complex<float> cmplx;
	std::minstd_rand rand(TAPS);
	std::uniform_real_distribution<float> dist(-1, 1);
	float kernel[TAPS];
	for (int i = 0; i < TAPS; ++i)
		kernel[i] = dist(rand);
	DSP::FIR<cmplx, float, TAPS> fir(kernel);
	const int LEN = 4 * TAPS;
	cmplx input[LEN];
	for (int n = 0; n < LEN; ++n) {
		input[n] = cmplx(dist(rand), dist(rand));
		double re = 0, im = 0;
		for (int k = 0; k < TAPS && k <= n; ++k) {
			re += double(kernel[k]) * double(input[n-k].real());
			im += double(kernel[k]) * double(input[n-k].imag());
		}
		cmplx output = fir(input[n]);
		if (std::abs(output.real() - re) > 0.0001 || std::abs(output.imag() - im) > 0.0001)
			return false;
	}
	return true;
}

int main()
{
	assert(integer_convolution_test<1>());
	assert(integer_convolution_test<2>());
	assert(integer_convolution_test<7>());
	assert(integer_convolution_test<64>());
	assert(integer_convolution_test<129>());
	assert(impulse_response_test<2>());
	assert(impulse_response_test<31>());
	assert(impulse_response_test<480>());
	assert(complex_convolution_test<3>());
	assert(complex_convolution_test<481>());
	std::cerr << "FIR filter test passed!" << std::endl;
	return 0;
}

