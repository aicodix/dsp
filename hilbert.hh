/*
Discrete Hilbert transformation

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "window.hh"
#include "bip_buffer.hh"

namespace DSP {

template <typename TYPE, int TAPS>
class Hilbert
{
	static_assert((TAPS-1) % 4 == 0, "TAPS-1 not divisible by four");
	typedef TYPE complex_type;
	typedef typename TYPE::value_type value_type;
	static const int LEN = (TAPS-1) / 2;
	// the taps at even distances from the middle are zero, so even and odd samples go into their own
	// history and each output reads only one of them densely, alternating with every sample
	BipBuffer<value_type, LEN> phase[2];
	value_type imco[LEN];
	value_type reco;
	int sel;
public:
	Hilbert(value_type a = value_type(2)) : sel(0)
	{
		Kaiser<value_type> win(a);
		reco = win((TAPS-1)/2, TAPS);
		for (int i = 0; i < LEN/2; ++i) {
			value_type c = win((2*i+1)+(TAPS-1)/2, TAPS) * 2 / ((2*i+1) * Const<value_type>::Pi());
			imco[LEN/2-1-i] = c;
			imco[LEN/2+i] = -c;
		}
	}
	complex_type operator()(value_type input)
	{
		const value_type *buf = phase[sel]();
		value_type re = reco * phase[!sel]()[LEN/2-1];
		value_type im = 0;
		for (int i = 0; i < LEN; ++i)
			im += imco[i] * buf[i];
		phase[sel](input);
		sel = !sel;
		return complex_type(re, im);
	}
};

}

