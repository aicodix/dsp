/*
Finite impulse response filter

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "bip_buffer.hh"

namespace DSP {

template <typename TYPE, typename VALUE, int TAPS>
class FIR
{
	BipBuffer<TYPE, TAPS> history;
	VALUE taps[TAPS];
public:
	template <typename KERNEL, typename WINDOW>
	FIR(KERNEL kernel, WINDOW window)
	{
		for (int i = 0; i < TAPS; ++i)
			taps[TAPS-1-i] = kernel(i, TAPS) * window(i, TAPS);
	}
	FIR(const VALUE *coeffs)
	{
		for (int i = 0; i < TAPS; ++i)
			taps[TAPS-1-i] = coeffs[i];
	}
	TYPE operator () (TYPE input)
	{
		const TYPE *buf = history(input);
		TYPE sum(0);
		for (int i = 0; i < TAPS; ++i)
			sum += taps[i] * buf[i];
		return sum;
	}
};

}

