/*
Repeated median estimator of Siegel

Copyright 2021 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include <algorithm>

namespace DSP {

template <typename TYPE, int SIZE>
class RepeatedMedianEstimator
{
	TYPE inner_[SIZE-1], outer_[SIZE];
	TYPE xint_, yint_, slope_;
public:
	RepeatedMedianEstimator() : xint_(0), yint_(0), slope_(0) {}
	void compute(const TYPE *x, const TYPE *y, int LEN)
	{
		if (LEN > SIZE)
			LEN = SIZE;
		for (int i = 0; i < LEN; ++i) {
			int count = 0;
			for (int j = 0; j < LEN; ++j)
				if (x[j] != x[i])
					inner_[count++] = (y[j] - y[i]) / (x[j] - x[i]);
			std::nth_element(inner_, inner_+count/2, inner_+count);
			outer_[i] = inner_[count/2];
		}
		std::nth_element(outer_, outer_+LEN/2, outer_+LEN);
		slope_ = outer_[LEN/2];
		for (int i = 0; i < LEN; ++i) {
			int count = 0;
			for (int j = 0; j < LEN; ++j)
				if (x[j] != x[i])
					inner_[count++] = (x[j]*y[i] - x[i]*y[j]) / (x[j] - x[i]);
			std::nth_element(inner_, inner_+count/2, inner_+count);
			outer_[i] = inner_[count/2];
		}
		std::nth_element(outer_, outer_+LEN/2, outer_+LEN);
		yint_ = outer_[LEN/2];
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
class RepeatedMedianEstimator2
{
	TYPE inner_[SIZE-1], outer_[SIZE];
	TYPE xint_, yint_, slope_;
public:
	RepeatedMedianEstimator2() : xint_(0), yint_(0), slope_(0) {}
	void compute(const TYPE *x, const TYPE *y, int LEN)
	{
		if (LEN > SIZE)
			LEN = SIZE;
		for (int i = 0; i < LEN; ++i) {
			int count = 0;
			for (int j = 0; j < LEN; ++j)
				if (x[j] != x[i])
					inner_[count++] = (y[j] - y[i]) / (x[j] - x[i]);
			std::nth_element(inner_, inner_+count/2, inner_+count);
			outer_[i] = inner_[count/2];
		}
		std::nth_element(outer_, outer_+LEN/2, outer_+LEN);
		slope_ = outer_[LEN/2];
		for (int i = 0; i < LEN; ++i)
			outer_[i] = y[i] - slope_ * x[i];
		std::nth_element(outer_, outer_+LEN/2, outer_+LEN);
		yint_ = outer_[LEN/2];
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

