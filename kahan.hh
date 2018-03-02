/*
Kahan summation algorithm

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef KAHAN_HH
#define KAHAN_HH

namespace DSP {

template <typename T>
class Kahan
{
	T high, low;
public:
	Kahan() : high(0), low(0) {}
	Kahan(T init) : high(init), low(0) {}
#if __clang__
	[[clang::optnone]]
#elif __GNUC__
	[[gnu::optimize("no-associative-math")]]
#else
#error unsupported compiler
#endif
	T operator ()(T input)
	{
		T tmp = input - low;
		T sum = high + tmp;
		low = (sum - high) - tmp;
		return high = sum;
	}
	T operator ()() { return high; }
};

template <class I, class T>
T kahan_sum(I begin, I end, T init)
{
	Kahan<T> kahan(init);
	while (begin != end)
		kahan(*begin++);
	return kahan();
}

}

#endif

