/*
Simple moving average

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "kahan.hh"

namespace DSP {

template <typename TYPE, typename VALUE, int NUM>
class SMA1
{
	TYPE hist_inp[NUM];
	TYPE hist_avg;
	int hist_pos;
public:
	SMA1() : hist_avg(0), hist_pos(0)
	{
		for (int i = 0; i < NUM; ++i)
			hist_inp[i] = 0;
	}
	VALUE abs_dev()
	{
		VALUE sum(abs(hist_inp[0] - hist_avg));
		for (int i = 1; i < NUM; ++i)
			sum += abs(hist_inp[i] - hist_avg);
		return sum / VALUE(NUM);
	}
	TYPE operator () (TYPE input)
	{
		hist_inp[hist_pos] = input;
		if (++hist_pos >= NUM)
			hist_pos = 0;
		TYPE hist_sum(hist_inp[0]);
		for (int i = 1; i < NUM; ++i)
			hist_sum += hist_inp[i];
		return hist_avg = hist_sum / VALUE(NUM);
	}
};

template <typename TYPE, typename VALUE, int NUM, bool NORM = true>
class SMA2
{
	TYPE hist_inp[NUM];
	TYPE hist_sum;
	int hist_pos;
public:
	SMA2() : hist_sum(0), hist_pos(0)
	{
		for (int i = 0; i < NUM; ++i)
			hist_inp[i] = 0;
	}
	TYPE operator () (TYPE input)
	{
		hist_sum += input - hist_inp[hist_pos];
		hist_inp[hist_pos] = input;
		if (++hist_pos >= NUM)
			hist_pos = 0;
		if (NORM)
			return hist_sum / VALUE(NUM);
		return hist_sum;
	}
};

template <typename TYPE, typename VALUE, int NUM, bool NORM = true>
class SMA3
{
	TYPE hist_inp[NUM];
	Kahan<TYPE> hist_sum;
	int hist_pos;
public:
	SMA3() : hist_sum(0), hist_pos(0)
	{
		for (int i = 0; i < NUM; ++i)
			hist_inp[i] = 0;
	}
	TYPE operator () (TYPE input)
	{
		hist_sum(-hist_inp[hist_pos]);
		hist_inp[hist_pos] = input;
		if (++hist_pos >= NUM)
			hist_pos = 0;
		if (NORM)
			return hist_sum(input) / VALUE(NUM);
		return hist_sum(input);
	}
};

template <typename TYPE, typename VALUE, int NUM, bool NORM = true>
class SMA4
{
	TYPE tree[2 * NUM];
	int leaf;
public:
	SMA4() : leaf(NUM)
	{
		for (int i = 0; i < 2 * NUM; ++i)
			tree[i] = 0;
	}
	TYPE operator () (TYPE input)
	{
		tree[leaf] = input;
		for (int child = leaf, parent = leaf / 2; parent; child = parent, parent /= 2)
			tree[parent] = tree[child] + tree[child^1];
		if (++leaf >= 2 * NUM)
			leaf = NUM;
		if (NORM)
			return tree[1] / VALUE(NUM);
		return tree[1];
	}
};

}

