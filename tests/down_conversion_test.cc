/*
Test for the down conversion

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
#include "phasor.hh"
#include "fir.hh"
#include "down_conversion.hh"

// compare to mixing with Phasor and filtering with FIR, keeping every DECIMATION-th output
template <typename TYPE, int TAPS, int DECIMATION>
bool reference_test(int n, int N)
{
	typedef DSP::Complex<TYPE> cmplx;
	DSP::LowPass<TYPE> kernel(TYPE(0.4) / TYPE(DECIMATION));
	DSP::Kaiser<TYPE> window(2);
	DSP::DownConversion<cmplx, TAPS, DECIMATION> ddc(n, N, kernel, window);
	DSP::Phasor<cmplx> osc;
	osc.omega(n, N);
	DSP::FIR<cmplx, TYPE, TAPS> fir(kernel, window);
	std::minstd_rand rand(TAPS + n);
	std::uniform_real_distribution<TYPE> dist(-1, 1);
	for (int m = 0; m < 4 * TAPS; ++m) {
		TYPE input[DECIMATION];
		cmplx expected;
		for (int i = 0; i < DECIMATION; ++i)
			expected = fir((input[i] = dist(rand)) * osc());
		cmplx output = ddc(input);
		if (std::abs(output.real() - expected.real()) > TYPE(0.0001) || std::abs(output.imag() - expected.imag()) > TYPE(0.0001))
			return false;
	}
	return true;
}

int main()
{
	assert((reference_test<float, 1, 1>(1, 4)));
	assert((reference_test<float, 31, 1>(-1500, 48000)));
	assert((reference_test<float, 481, 1>(-1500, 48000)));
	assert((reference_test<float, 32, 1>(3, 7)));
	assert((reference_test<float, 129, 2>(-1500, 44100)));
	assert((reference_test<float, 129, 3>(5, 17)));
	assert((reference_test<double, 481, 8>(-1500, 48000)));
	assert((reference_test<double, 64, 5>(1500, 8000)));
	DSP::DownConversion<DSP::Complex<float>, 31> single(1, 4, DSP::LowPass<float>(0.2f), DSP::Kaiser<float>(2));
	assert(std::abs(single(1.f).real()) < 1);
	std::cerr << "Down conversion test passed!" << std::endl;
	return 0;
}
