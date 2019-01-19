/*
Coefficient array helper

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace DSP {

template <typename TYPE>
struct CoeffsFunc
{
	virtual TYPE operator () (int, int) = 0;
	virtual ~CoeffsFunc() = default;
};

template <int TAPS, typename TYPE>
class Coeffs
{
	TYPE w[TAPS];
public:
	Coeffs(CoeffsFunc<TYPE> *func)
	{
		for (int n = 0; n < TAPS; ++n)
			w[n] = (*func)(n, TAPS);
	}
	Coeffs(CoeffsFunc<TYPE> *func0, CoeffsFunc<TYPE> *func1)
	{
		for (int n = 0; n < TAPS; ++n)
			w[n] = (*func0)(n, TAPS) * (*func1)(n, TAPS);
	}
	void normalize(TYPE divisor = 1)
	{
		TYPE sum(0);
		for (int n = 0; n < TAPS; ++n)
			sum += w[n];
		for (int n = 0; n < TAPS; ++n)
			w[n] /= divisor * std::abs(sum);
	}
	inline TYPE operator () (int n) { return n >= 0 && n < TAPS ? w[n] : 0; }
	inline operator const TYPE * () const { return w; }
};

}

