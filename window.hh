/*
Some window functions

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef WINDOW_HH
#define WINDOW_HH

namespace DSP {

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

}

#endif

