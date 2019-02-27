/*
Frequency-domain zero padding

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "fft.hh"

namespace DSP {

template <int BINS, int FACT, typename CMPLX>
class FDZP
{
	typedef typename CMPLX::value_type VALUE;
	FastFourierTransform<BINS, CMPLX, -1> fwd;
	FastFourierTransform<BINS * FACT, CMPLX, 1> bwd;
	CMPLX tmp[BINS * FACT];
	static constexpr VALUE SCALE = VALUE(1) / VALUE(BINS);
public:
	void operator ()(CMPLX *output, const CMPLX *input)
	{
		fwd(tmp, input);
		if (BINS&1) {
			for (int i = 1; i < BINS/2; ++i)
				tmp[BINS*FACT-BINS/2+i] = tmp[BINS/2+1+i];
			for (int i = 1; i < BINS/2; ++i)
				tmp[BINS/2+1+i] = 0;
		} else {
			tmp[BINS/2] *= VALUE(0.5);
			tmp[BINS*FACT-BINS/2] = tmp[BINS/2];
			for (int i = 1; i < BINS/2; ++i)
				tmp[BINS*FACT-BINS/2+i] = tmp[BINS/2+i];
			for (int i = 1; i < BINS/2; ++i)
				tmp[BINS/2+i] = 0;
		}
		bwd(output, tmp);
		for (int i = 0; i < BINS * FACT; ++i)
			output[i] *= SCALE;
	}
};

}

