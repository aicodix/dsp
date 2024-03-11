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
static inline void median(TYPE *A, int a, int b, int c, int d, int e)
{
	if (A[c] < A[a])
		swap(A, a, c);
	if (A[d] < A[b])
		swap(A, b, d);
	if (A[d] < A[c]) {
		swap(A, c, d);
		swap(A, a, b);
	}
	if (A[e] < A[b])
		swap(A, b, e);
	if (A[e] < A[c]) {
		swap(A, c, e);
		if (A[c] < A[a])
			swap(A, a, c);
	} else if (A[c] < A[b]) {
		swap(A, b, c);
	}
}

template <typename TYPE>
static void partition(TYPE *a, int &l, int &h)
{
	int half = (h - l) / 2;
	int quarter = (h - l) / 4;
	int middle = l + half;
	median(a, l, l + quarter, middle, middle + quarter, h);
	TYPE pivot = a[middle];
	for (int i = l; i <= h;)
		if (a[i] < pivot)
			swap(a, i++, l++);
		else if (a[i] > pivot)
			swap(a, i, h--);
		else
			++i;
}

template <typename TYPE>
static void select(TYPE *a, int l, int h, int k)
{
	while (l < h) {
		int lt = l, gt = h;
		partition(a, lt, gt);
		if (k < lt)
			h = lt - 1;
		else if (k > gt)
			l = gt + 1;
		else
			break;
	}
}

}

template <typename TYPE>
TYPE quick_select(TYPE *a, int k, int n)
{
	assert(n && k < n);
	QUICK::select(a, 0, n-1, k);
	return a[k];
}

}

