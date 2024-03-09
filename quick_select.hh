/*
Quick select algorithm

Copyright 2024 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace DSP {

namespace QUICK {

template <typename TYPE>
static inline void swap(TYPE *a, int i, int j)
{
	TYPE t = a[i];
	a[i] = a[j];
	a[j] = t;
}

template <typename TYPE>
static int partition(TYPE *a, int l, int h)
{
	int p = (l + h) / 2;
	swap(a, p, h);
	for (int i = l; i < h; ++i)
		if (a[i] < a[h])
			swap(a, i, l++);
	swap(a, l, h);
	return l;
}

template <typename TYPE>
static TYPE select(TYPE *a, int l, int h, int k)
{
	while (l < h) {
		int i = partition(a, l, h);
		if (k == i)
			return a[k];
		if (k < i)
			h = i - 1;
		else
			l = i + 1;
	}
	return a[l];
}

}

template <typename TYPE>
TYPE quick_select(TYPE *a, int k, int n)
{
	assert(n && k < n);
	return QUICK::select(a, 0, n-1, k);
}

}

