/*
Some window functions

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef WINDOW_HH
#define WINDOW_HH

#include "const.hh"
#include "kahan.hh"

namespace DSP {

template <typename TYPE>
struct Window
{
	virtual TYPE operator () (int, int) = 0;
};

template <int TAPS, typename TYPE>
class Taps
{
	TYPE w[TAPS];
public:
	Taps(Window<TYPE> *func)
	{
		for (int n = 0; n < TAPS; ++n)
			w[n] = (*func)(n, TAPS);
	}
	void normalize()
	{
		TYPE sum(0);
		for (int n = 0; n < TAPS; ++n)
			sum += w[n];
		for (int n = 0; n < TAPS; ++n)
			w[n] /= std::abs(sum);
	}
	inline TYPE operator () (int n) { return n >= 0 && n < TAPS ? w[n] : 0; }
	inline operator const TYPE * () const { return w; }
};

template <typename TYPE>
struct Rect : public Window<TYPE>
{
	TYPE operator () (int n, int N) { return n >= 0 && n < N ? 1 : 0; }
};

template <typename TYPE>
struct Hann : public Window<TYPE>
{
	TYPE operator () (int n, int N)
	{
		return TYPE(0.5) * (TYPE(1) - std::cos(Const<TYPE>::TwoPi() * TYPE(n) / TYPE(N - 1)));
	}
};

template <typename TYPE>
struct Hamming : public Window<TYPE>
{
	TYPE operator () (int n, int N)
	{
		return TYPE(0.54) - TYPE(0.46) * std::cos(Const<TYPE>::TwoPi() * TYPE(n) / TYPE(N - 1));
	}
};

template <typename TYPE>
class Lanczos : public Window<TYPE>
{
	static TYPE sinc(TYPE x)
	{
		return TYPE(0) == x ? TYPE(1) : std::sin(Const<TYPE>::Pi() * x) / (Const<TYPE>::Pi() * x);
	}
public:
	TYPE operator () (int n, int N)
	{
		return sinc(TYPE(2 * n) / TYPE(N - 1) - TYPE(1));
	}
};

template <typename TYPE>
class Blackman : public Window<TYPE>
{
	TYPE a0, a1, a2;
public:
	Blackman(TYPE a0, TYPE a1, TYPE a2) : a0(a0), a1(a1), a2(a2) {}
	Blackman(TYPE a) : Blackman((TYPE(1) - a) / TYPE(2), TYPE(0.5), a / TYPE(2)) {}
	// "exact Blackman"
	Blackman() : Blackman(TYPE(7938) / TYPE(18608), TYPE(9240) / TYPE(18608), TYPE(1430) / TYPE(18608)) {}
	TYPE operator () (int n, int N)
	{
		return a0 - a1 * std::cos(Const<TYPE>::TwoPi() * TYPE(n) / TYPE(N - 1)) + a2 * std::cos(Const<TYPE>::FourPi() * TYPE(n) / TYPE(N - 1));
	}
};

template <typename TYPE>
class Gauss : public Window<TYPE>
{
	TYPE o;
public:
	Gauss(TYPE o) : o(o) {}
	TYPE operator () (int n, int N)
	{
		return std::exp(- TYPE(0.5) * std::pow((TYPE(n) - TYPE(N - 1) / TYPE(2)) / (o * TYPE(N - 1) / TYPE(2)), TYPE(2)));
	}
};

template <typename TYPE>
class Kaiser : public Window<TYPE>
{
	TYPE a;
	/*
	i0() implements the zero-th order modified Bessel function of the first kind:
	https://en.wikipedia.org/wiki/Bessel_function#Modified_Bessel_functions:_I%CE%B1,_K%CE%B1
	$I_\alpha(x) = i^{-\alpha} J_\alpha(ix) = \sum_{m=0}^\infty \frac{1}{m!\, \Gamma(m+\alpha+1)}\left(\frac{x}{2}\right)^{2m+\alpha}$
	$I_0(x) = J_0(ix) = \sum_{m=0}^\infty \frac{1}{m!\, \Gamma(m+1)}\left(\frac{x}{2}\right)^{2m} = \sum_{m=0}^\infty \left(\frac{x^m}{2^m\,m!}\right)^{2}$
	We obviously can't use the factorial here, so let's get rid of it:
	$= 1 + \left(\frac{x}{2 \cdot 1}\right)^2 + \left(\frac{x}{2 \cdot 1}\cdot \frac{x}{2 \cdot 2}\right)^2 + \left(\frac{x}{2 \cdot 1}\cdot \frac{x}{2 \cdot 2}\cdot \frac{x}{2 \cdot 3}\right)^2 + .. = 1 + \sum_{m=1}^\infty \left(\prod_{n=1}^m \frac{x}{2n}\right)^2$
	*/
	static TYPE i0(TYPE x)
	{
		Kahan<TYPE> sum(1.0);
		TYPE val = 1.0;
		// converges for -3*Pi:3*Pi in less than:
		// float: 25 iterations
		// double: 35 iterations
		for (int n = 1; n < 35; ++n) {
			val *= x / TYPE(2 * n);
			if (sum.same(val * val))
				return sum();
		}
		return sum();
	}
public:
	Kaiser(TYPE a) : a(a) {}
	TYPE operator () (int n, int N)
	{
		return i0(Const<TYPE>::Pi() * a * std::sqrt(TYPE(1) - std::pow(TYPE(2 * n) / TYPE(N - 1) - TYPE(1), TYPE(2)))) / i0(Const<TYPE>::Pi() * a);
	}
};

}

#endif

