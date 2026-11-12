/*
Down conversion with optional decimation

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "bip_buffer.hh"
#include "phasor.hh"
#include "unit_circle.hh"

namespace DSP {

/*
Mixing with the oscillator e^(jwn) and then filtering with the low-pass h is the same as
filtering with the band-pass h[k] e^(-jwk) and then mixing, which needs only the
outputs we keep and keeps the history real.
*/
template <typename TYPE, int TAPS, int DECIMATION = 1>
class DownConversion
{
	typedef TYPE complex_type;
	typedef typename TYPE::value_type value_type;
	BipBuffer<value_type, TAPS> history;
	// two arrays of reals instead of complex taps, so the loop is two plain dot products without shuffles
	value_type real[TAPS], imag[TAPS];
	Phasor<complex_type> osc;
public:
	// mixes with e^(2 pi j n / N), like Phasor::omega(n, N), and then filters with kernel * window
	template <typename KERNEL, typename WINDOW>
	DownConversion(int n, int N, KERNEL kernel, WINDOW window)
	{
		n %= N;
		if (n < 0)
			n += N;
		// the output belongs to the newest of the DECIMATION samples, so the taps also turn by w (DECIMATION - 1)
		for (int k = 0, a = int((long(n) * (DECIMATION - 1)) % N); k < TAPS; ++k, a = (a + N - n) % N) {
			value_type h = kernel(k, TAPS) * window(k, TAPS);
			real[TAPS-1-k] = h * UnitCircle<value_type>::cos(a, N);
			imag[TAPS-1-k] = h * UnitCircle<value_type>::sin(a, N);
		}
		osc.omega(int((long(n) * DECIMATION) % N), N);
	}
	// takes DECIMATION samples and gives one
	complex_type operator () (const value_type *input)
	{
		const value_type *buf = nullptr;
		for (int i = 0; i < DECIMATION; ++i)
			buf = history(input[i]);
		value_type re = 0, im = 0;
		for (int i = 0; i < TAPS; ++i) {
			re += real[i] * buf[i];
			im += imag[i] * buf[i];
		}
		return osc() * complex_type(re, im);
	}
	complex_type operator () (value_type input)
	{
		static_assert(DECIMATION == 1, "takes DECIMATION samples");
		return operator () (&input);
	}
};

}
