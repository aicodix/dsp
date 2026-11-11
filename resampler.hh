/*
Resampling by an arbitrary rate difference

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "window.hh"
#include "spline.hh"
#include "utils.hh"
#include "const.hh"

namespace DSP {

template <typename TYPE, int TAPS, int OVER>
class Resampler
{
	// the kernel is evaluated from -0.5 to TAPS - 0.5, centered at (TAPS - 1) / 2, so its
	// table goes from -1 to TAPS, which keeps knots at whole samples and the window centered
	static const int KNOTS = (TAPS + 1) * OVER + 1;
	typedef DSP::UniformNaturalCubicSpline<KNOTS, TYPE, TYPE> spline_type;
	spline_type lpf;
	TYPE rate;
public:
	Resampler(TYPE rate, TYPE cutoff, TYPE alpha) : rate(rate)
	{
		TYPE tmp[KNOTS];
		DSP::Kaiser<TYPE> window(alpha);
		for (int n = 0; n < KNOTS; ++n) {
			TYPE x = TYPE(n) / TYPE(OVER) - TYPE(TAPS + 1) / TYPE(2);
			TYPE y = TYPE(2) * cutoff / rate * sinc(TYPE(2) * cutoff / rate * x);
			tmp[n] = y * window(n, KNOTS);
		}
		lpf = spline_type(tmp, TYPE(-1), TYPE(1) / TYPE(OVER));
	}
	template <typename IO>
	void operator ()(IO *output, const IO *input, TYPE diff, int samples, int stride = 1)
	{
		double ratio = rate / (rate + diff);
		for (int i = 0; i < samples - TAPS; ++i) {
			IO sum = 0;
			double x = i * ratio;
			int s0 = nearbyint(x);
			int s1 = s0 + TAPS;
			if (s0 < 0 || s1 > samples)
				continue;
			for (int s = s0; s < s1; ++s) {
				TYPE k = s - x;
				sum += lpf(k) * input[s * stride];
			}
			output[(i + (TAPS - 1) / 2) * stride] = sum;
		}
	}
};

template <typename TYPE, typename IO, int TAPS, int OVER>
class Resampler2
{
	// the kernel is evaluated from -0.5 to TAPS - 0.5, centered at (TAPS - 1) / 2, so its
	// table goes from -1 to TAPS, which keeps knots at whole samples and the window centered
	static const int KNOTS = (TAPS + 1) * OVER + 1;
	typedef DSP::UniformNaturalCubicSpline<KNOTS, TYPE, TYPE> spline_type;
	spline_type lpf;
	TYPE rate;
	TYPE dpos;
	IO buf[TAPS];
public:
	Resampler2(TYPE rate, TYPE cutoff, TYPE alpha) : rate(rate), dpos(0)
	{
		TYPE tmp[KNOTS];
		DSP::Kaiser<TYPE> window(alpha);
		for (int n = 0; n < KNOTS; ++n) {
			TYPE x = TYPE(n) / TYPE(OVER) - TYPE(TAPS + 1) / TYPE(2);
			TYPE y = TYPE(2) * cutoff / rate * sinc(TYPE(2) * cutoff / rate * x);
			tmp[n] = y * window(n, KNOTS);
		}
		lpf = spline_type(tmp, TYPE(-1), TYPE(1) / TYPE(OVER));
		for (int i = 0; i < TAPS; ++i)
			buf[i] = 0;
	}
	int operator ()(IO *output, const IO input, TYPE diff)
	{
		for (int i = 0; i < TAPS-1; ++i)
			buf[i] = buf[i+1];
		buf[TAPS-1] = input;
		if (dpos < TYPE(-0.5)) {
			dpos += TYPE(1);
			return 0;
		}
		// the output rate is rate + diff, as with Resampler
		TYPE delta = diff / (rate + diff);
		int samples = 0;
		again:
		IO sum = 0;
		for (int s = 0; s < TAPS; ++s) {
			TYPE k = s + dpos;
			sum += lpf(k) * buf[s];
		}
		output[samples++] = sum;
		dpos += delta;
		if (dpos > TYPE(0.5)) {
			dpos -= TYPE(1);
			goto again;
		}
		return samples;
	}
};

}

