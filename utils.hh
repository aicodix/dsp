/*
Some little helpers

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef UTILS_HH
#define UTILS_HH

#include "const.hh"

namespace DSP {

template <typename TYPE>
int signum(TYPE v)
{
	return (v > TYPE(0)) - (v < TYPE(0));
}

template <typename X, typename AB>
AB lerp(X x, AB a, AB b)
{
	return (X(1) - x) * a + x * b;
}

template <typename TYPE>
TYPE normal_pdf(TYPE x, TYPE m, TYPE s)
{
	return std::exp(-std::pow((x - m) / s, TYPE(2)) / TYPE(2)) / (Const<TYPE>::SqrtTwoPi() * s);
}

}

#endif

