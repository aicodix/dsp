/*
Resampling by an arbitrary rate difference

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/


#ifndef RESAMPLER_HH
#define RESAMPLER_HH

#include "window.hh"
#include "spline.hh"
#include "utils.hh"
#include "const.hh"

namespace DSP {

template <typename TYPE, int RATE, int TAPS, int OVER>
class Resampler
{
	typedef DSP::UniformNaturalCubicSpline<TAPS * OVER, TYPE, TYPE> spline_type;
	spline_type lpf;
public:
	Resampler(TYPE cutoff, TYPE alpha)
	{
		TYPE tmp[TAPS * OVER];
		DSP::Kaiser<TYPE> window(alpha);
		for (int n = 0; n < TAPS * OVER; ++n) {
			TYPE x = TYPE(n) / TYPE(OVER) - TYPE(TAPS - 1) / TYPE(2);
			TYPE y = TYPE(2) * cutoff / TYPE(RATE) * sinc(TYPE(2) * cutoff / TYPE(RATE) * x);
			tmp[n] = y * window(n, TAPS * OVER);
		}
		lpf = spline_type(tmp, 0, 1.0 / OVER);
	}
	template <typename IO>
	void operator ()(IO *output, IO *input, TYPE diff, int samples, int stride = 1)
	{
		TYPE ratio = (RATE + diff) / RATE;
		TYPE recip = RATE / (RATE + diff);
		for (int i = 0; i < samples - TAPS; ++i) {
			IO sum = 0;
			int s0 = nearbyint(i * ratio);
			int s1 = s0 + TAPS;
			for (int s = s0; s < s1; ++s) {
				TYPE k = s * recip - i;
				sum += lpf(k) * input[s * stride];
			}
			output[(i + (TAPS - 1) / 2) * stride] = sum;
		}
	}
};

}

#endif

