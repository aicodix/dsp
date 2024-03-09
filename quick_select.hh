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
static inline void sort(TYPE *a, int i, int j, int k)
{
	if (a[i] > a[j])
		swap(a, i, j);
	if (a[i] > a[k])
		swap(a, i, k);
	if (a[j] > a[k])
		swap(a, j, k);
}

template <typename TYPE>
static int partition(TYPE *a, int l, int h)
{
	sort(a, (l + h) / 2, h, l);
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

