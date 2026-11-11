/*
Test for the resamplers

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include <cmath>
namespace DSP { using std::abs; using std::sqrt; using std::sin; using std::cos; using std::nearbyint; }
#include "complex.hh"
#include "resampler.hh"

typedef double value;
typedef DSP::Complex<value> cmplx;

const value rate = 48000;
const int LEN = 50000;
const int SKIP = 1000;
static cmplx input[LEN], output[2 * LEN];

static cmplx tone(value freq, value time)
{
	value phase = 2 * M_PI * freq * time / rate;
	return cmplx(cos(phase), sin(phase));
}

// power of the error against the ideal tone, at output spacing "step" and some delay, in dB
// the delay is searched on a coarse grid and the phase taken from the correlation
static value error(const cmplx *out, int begin, int end, value freq, value step, value delay_min, value delay_max)
{
	value best = 0;
	for (value delay = delay_min; delay <= delay_max; delay += value(0.5)) {
		cmplx corr = 0;
		for (int k = begin; k < end; ++k)
			corr += out[k] * conj(tone(freq, delay + k * step));
		value err = 0;
		for (int k = begin; k < end; ++k)
			err += norm(out[k] - tone(freq, delay + k * step) * corr / abs(corr));
		err /= end - begin;
		if (delay == delay_min || err < best)
			best = err;
	}
	return 10 * std::log10(best);
}

template <int TAPS, int OVER>
value resampler2_error(value freq, value ppm)
{
	DSP::Resampler2<value, cmplx, TAPS, OVER> resample(rate, rate * 19 / 40, 2);
	value diff = rate * ppm / 1000000;
	int count = 0;
	for (int n = 0; n < LEN; ++n)
		count += resample(output + count, tone(freq, n), diff);
	// the output rate is rate + diff
	value step = rate / (rate + diff);
	return error(output, SKIP, count - SKIP, freq, step, - TAPS, 0);
}

template <int TAPS, int OVER>
value resampler_error(value freq, value ppm)
{
	DSP::Resampler<value, TAPS, OVER> resample(rate, rate * 19 / 40, 2);
	for (int n = 0; n < LEN; ++n)
		input[n] = tone(freq, n);
	for (int n = 0; n < LEN; ++n)
		output[n] = 0;
	value diff = rate * ppm / 1000000;
	resample(output, input, diff, LEN);
	value step = rate / (rate + diff);
	// output i + (TAPS - 1) / 2 is the input at i * step + (TAPS - 1) / 2
	return error(output, SKIP, LEN - 2 * SKIP, freq, step, value((TAPS - 1) / 2) * (1 - step) - 2, value((TAPS - 1) / 2) * (1 - step) + 2);
}

int main()
{
	// up to an eighth of the rate, the kernel of 129 taps, 3 knots per tap and a Kaiser window of alpha 2 is
	// good to below -85 dB, at a quarter of the rate, where its spline gets coarse, to below -75 dB
	for (value freq: {100, 500, 1500, 3000, 6000, 12000}) {
		value limit = freq > rate / 8 ? -75 : -85;
		for (value ppm: {-2000, -300, 0, 300, 2000}) {
			value e2 = resampler2_error<129, 3>(freq, ppm);
			value e1 = resampler_error<129, 3>(freq, ppm);
			std::cout << freq << " Hz " << ppm << " ppm: Resampler2 " << e2 << " dB, Resampler " << e1 << " dB" << std::endl;
			assert(e2 < limit && e1 < limit);
		}
	}
	std::cerr << "Resampler test passed!" << std::endl;
	return 0;
}
