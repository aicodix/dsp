/*
Fs/4 Complex down conversion

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "filter.hh"
#include "window.hh"
#include "bip_buffer.hh"

namespace DSP {

template <typename TYPE, int TAPS>
class CDC
{
	static_assert((TAPS-1) % 4 == 0, "TAPS-1 not divisible by four");
	typedef TYPE complex_type;
	typedef typename TYPE::value_type value_type;
	BipBuffer<value_type, (TAPS-1)/4> real;
	BipBuffer<value_type, (TAPS-1)/2> imag;
	value_type imco[(TAPS-1)/4];
	value_type reco;
	bool sign;
public:
	CDC(value_type a = value_type(2)) : sign(false)
	{
		Kaiser<value_type> win(a);
		LowPass2<value_type> hbf(1, 4);
		reco = win((TAPS-1)/2, TAPS) * hbf((TAPS-1)/2, TAPS);
		for (int i = 0; i < (TAPS-1)/4; ++i)
			imco[i] = win(2*i+(TAPS-1)/2+1, TAPS) * hbf(2*i+(TAPS-1)/2+1, TAPS);
	}
	complex_type operator()(value_type in0, value_type in1)
	{
		sign = !sign;
		value_type re = reco * real()[0];
		real(sign ? in1 : -in1);
		const value_type *buf = imag(sign ? in0 : -in0);
		value_type im = imco[0] * (buf[(TAPS-1)/4-1] + buf[(TAPS-1)/4]);
		for (int i = 1; i < (TAPS-1)/4; ++i)
			im += imco[i] * (buf[(TAPS-1)/4-1-i] + buf[(TAPS-1)/4+i]);
		return complex_type(re, im);
	}
};

}

