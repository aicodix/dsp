/*
Theil–Sen estimator

Copyright 2021 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

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

}

