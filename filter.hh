/*
Some finite impulse response filter functions

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "const.hh"
#include "utils.hh"
#include "coeffs.hh"

namespace DSP {

template <typename TYPE>
class LowPass : public CoeffsFunc<TYPE>
{
	TYPE f;
public:
	LowPass(TYPE cutoff) : f(TYPE(2) * cutoff) {}
	TYPE operator () (int n, int N)
	{
		TYPE x = TYPE(n) - TYPE(0.5) * TYPE(N - 1);
		return f * sinc(f * x);
	}
};

template <typename TYPE>
class HighPass : public CoeffsFunc<TYPE>
{
	TYPE f;
public:
	HighPass(TYPE cutoff) : f(TYPE(2) * cutoff) {}
	TYPE operator () (int n, int N)
	{
		TYPE x = TYPE(n) - TYPE(0.5) * TYPE(N - 1);
		// if (N%1) return delta(x) - f * sinc(f * x);
		return sinc(x) - f * sinc(f * x);

	}
};

template <typename TYPE>
class BandPass : public CoeffsFunc<TYPE>
{
	TYPE f0, f1;
public:
	BandPass(TYPE cutoff0, TYPE cutoff1) :
		f0(TYPE(2) * cutoff0), f1(TYPE(2) * cutoff1) {}
	TYPE operator () (int n, int N)
	{
		TYPE x = TYPE(n) - TYPE(0.5) * TYPE(N - 1);
		return f1 * sinc(f1 * x) - f0 * sinc(f0 * x);
	}
};

}

