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
static void partition(TYPE *a, int &l, int &h)
{
	int m = l + (h - l) / 2;
	sort(a, l, m, h);
	TYPE pivot = a[m];
	for (int i = l; i <= h;)
		if (a[i] < pivot)
			swap(a, i++, l++);
		else if (a[i] > pivot)
			swap(a, i, h--);
		else
			++i;
}

template <typename TYPE>
static TYPE select(TYPE *a, int l, int h, int k)
{
	while (l < h) {
		int lt = l, gt = h;
		partition(a, lt, gt);
		if (k < lt)
			h = lt - 1;
		else if (k > gt)
			l = gt + 1;
		else
			return a[k];
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

