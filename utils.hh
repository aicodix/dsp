/*
Some little helpers

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef UTILS_HH
#define UTILS_HH

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

}

#endif

