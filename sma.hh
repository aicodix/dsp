/*
Simple moving average

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

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

template <typename TYPE, typename VALUE, int NUM>
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
		return hist_sum / VALUE(NUM);
	}
};

}

