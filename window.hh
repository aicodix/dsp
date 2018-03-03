/*
Some window functions

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef WINDOW_HH
#define WINDOW_HH

#include "kahan.hh"

namespace DSP {

template <int TAPS, typename TYPE>
class Rect
{
	TYPE w[TAPS];
public:
	Rect()
	{
		for (int n = 0; n < TAPS; ++n)
			w[n] = TYPE(1);
	}
	inline TYPE operator () (int n) { return n >= 0 && n < TAPS ? w[n] : 0; }
	inline operator const TYPE * () const { return w; }
};

template <int TAPS, typename TYPE>
class Hann
{
	TYPE w[TAPS];
public:
	Hann()
	{
		for (int n = 0; n < TAPS; ++n)
			w[n] = TYPE(0.5) * (TYPE(1) - std::cos(TYPE(2) * TYPE(M_PI) * TYPE(n) / TYPE(TAPS - 1)));
	}
	inline TYPE operator () (int n) { return n >= 0 && n < TAPS ? w[n] : 0; }
	inline operator const TYPE * () const { return w; }
};

template <int TAPS, typename TYPE>
class Hamming
{
	TYPE w[TAPS];
public:
	Hamming()
	{
		for (int n = 0; n < TAPS; ++n)
			w[n] = TYPE(0.54) - TYPE(0.46) * std::cos(TYPE(2) * TYPE(M_PI) * TYPE(n) / TYPE(TAPS - 1));
	}
	inline TYPE operator () (int n) { return n >= 0 && n < TAPS ? w[n] : 0; }
	inline operator const TYPE * () const { return w; }
};

template <int TAPS, typename TYPE>
class Lanczos
{
	TYPE w[TAPS];
	TYPE sinc(TYPE x)
	{
		return TYPE(0) == x ? TYPE(1) : std::sin(TYPE(M_PI) * x) / (TYPE(M_PI) * x);
	}
public:
	Lanczos()
	{
		for (int n = 0; n < TAPS; ++n)
			w[n] = sinc(TYPE(2 * n) / TYPE(TAPS - 1) - TYPE(1));
	}
	inline TYPE operator () (int n) { return n >= 0 && n < TAPS ? w[n] : 0; }
	inline operator const TYPE * () const { return w; }
};

template <int TAPS, typename TYPE>
class Gauss
{
	TYPE w[TAPS];
public:
	Gauss(TYPE o)
	{
		for (int n = 0; n < TAPS; ++n)
			w[n] = std::exp(- TYPE(0.5) * std::pow((TYPE(n) - TYPE(TAPS - 1) / TYPE(2)) / (o * TYPE(TAPS - 1) / TYPE(2)), TYPE(2)));
	}
	inline TYPE operator () (int n) { return n >= 0 && n < TAPS ? w[n] : 0; }
	inline operator const TYPE * () const { return w; }
};

template <int TAPS, typename TYPE>
class Kaiser
{
	TYPE w[TAPS];
	TYPE i0(TYPE x)
	{
		Kahan<TYPE> sum(1.0);
		TYPE val = 1.0;
		// converges for -3*M_PI:3*M_PI in less than:
		// float: 25 iterations
		// double: 35 iterations
		for (int n = 1; n < 35; ++n) {
			TYPE tmp = x / TYPE(2 * n);
			if (sum.same(val *= tmp * tmp))
				return sum();
		}
		return sum();
	}
public:
	Kaiser(TYPE a)
	{
		for (int n = 0; n < TAPS; ++n)
			w[n] = i0(TYPE(M_PI) * a * std::sqrt(TYPE(1) - std::pow(TYPE(2 * n) / TYPE(TAPS - 1) - TYPE(1), TYPE(2)))) / i0(TYPE(M_PI) * a);
	}
	inline TYPE operator () (int n) { return n >= 0 && n < TAPS ? w[n] : 0; }
	inline operator const TYPE * () const { return w; }
};

}

#endif

