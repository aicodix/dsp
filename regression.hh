/*
Regression analysis

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef REGRESSION_HH
#define REGRESSION_HH

namespace DSP {

template <typename TYPE>
class SimpleLinearRegression
{
	TYPE avgX, avgY;
	TYPE varX, covXY;
public:
	SimpleLinearRegression(TYPE *x, TYPE *y, int LEN) : avgX(0), avgY(0), varX(0), covXY(0)
	{
		for (int i = 0; i < LEN; ++i) {
			avgX += x[i];
			avgY += y[i];
		}
		avgX /= LEN;
		avgY /= LEN;
		for (int i = 0; i < LEN; ++i) {
			varX += (x[i] - avgX) * (x[i] - avgX);
			covXY += (x[i] - avgX) * (y[i] - avgY);
		}
	}
	TYPE xint()
	{
		return avgX - avgY * varX / covXY;
	}
	TYPE slope()
	{
		return covXY / varX;
	}
	TYPE yint()
	{
		return avgY - slope() * avgX;
	}
	TYPE operator () (TYPE x)
	{
		return yint() + slope() * x;
	}
};

}

#endif

