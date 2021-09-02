/*
Theil–Sen estimator

Copyright 2021 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include <random>
#include <functional>
#include <algorithm>

namespace DSP {

template <typename TYPE, int LEN_MAX>
class TheilSenEstimator
{
	static const int size_ = ((LEN_MAX-1)*LEN_MAX)/2;
	TYPE temp_[size_];
	TYPE xint_, yint_, slope_;
public:
	TheilSenEstimator() : xint_(0), yint_(0), slope_(0) {}
	void compute(TYPE *x, TYPE *y, int LEN)
	{
		int count = 0;
		for (int i = 0; count < size_ && i < LEN; ++i)
			for (int j = i+1; count < size_ && j < LEN; ++j)
				if (x[j] != x[i])
					temp_[count++] = (y[j] - y[i]) / (x[j] - x[i]);
		std::nth_element(temp_, temp_+count/2, temp_+count);
		slope_ = temp_[count/2];
		count = 0;
		for (int i = 0; count < size_ && i < LEN; ++i)
			temp_[count++] = y[i] - slope_ * x[i];
		std::nth_element(temp_, temp_+count/2, temp_+count);
		yint_ = temp_[count/2];
		xint_ = - yint_ / slope_;
	}
	TYPE xint()
	{
		return xint_;
	}
	TYPE slope()
	{
		return slope_;
	}
	TYPE yint()
	{
		return yint_;
	}
	TYPE operator () (TYPE x)
	{
		return yint_ + slope_ * x;
	}
};

template <typename TYPE, int SIZE>
class TheilSenEstimator2
{
	TYPE temp_[SIZE];
	TYPE xint_, yint_, slope_;
public:
	TheilSenEstimator2() : xint_(0), yint_(0), slope_(0) {}
	void compute(TYPE *x, TYPE *y, int LEN)
	{
		std::random_device rd;
		std::default_random_engine generator(rd());
		typedef std::uniform_int_distribution<int> uniform;
		auto rand = std::bind(uniform(0, LEN-1), generator);
		for (int i, j, k = 0; k < SIZE; ++k) {
			while (x[i=rand()] == x[j=rand()]);
			temp_[k] = (y[j] - y[i]) / (x[j] - x[i]);
		}
		std::nth_element(temp_, temp_+SIZE/2, temp_+SIZE);
		slope_ = temp_[SIZE/2];
		int count = std::min(LEN, SIZE);
		for (int i = 0; i < count; ++i)
			temp_[i] = y[i] - slope_ * x[i];
		std::nth_element(temp_, temp_+count/2, temp_+count);
		yint_ = temp_[count/2];
		xint_ = - yint_ / slope_;
	}
	TYPE xint()
	{
		return xint_;
	}
	TYPE slope()
	{
		return slope_;
	}
	TYPE yint()
	{
		return yint_;
	}
	TYPE operator () (TYPE x)
	{
		return yint_ + slope_ * x;
	}
};

}

