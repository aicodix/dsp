/*
Some spline algorithms

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef SPLINE_HH
#define SPLINE_HH

namespace DSP {

template <int KNOTS, typename OTYPE, typename ITYPE>
class UniformNaturalCubicSpline
{
	OTYPE A[KNOTS], B[KNOTS], C[KNOTS], D[KNOTS];
public:
	UniformNaturalCubicSpline(OTYPE *y, int STRIDE = 1)
	{
		for (int i = 0; i < KNOTS; ++i)
			A[i] = y[i * STRIDE];
		ITYPE u[KNOTS], l[KNOTS];
		u[0] = 0;
		l[0] = l[KNOTS-1] = 1;
		OTYPE z[KNOTS];
		z[0] = z[KNOTS-1] = 0;
		for (int i = 1; i < KNOTS - 1; ++i) {
			l[i] = ITYPE(4) - u[i-1];
			u[i] = ITYPE(1) / l[i];
			z[i] = (ITYPE(3) * (A[i+1] - ITYPE(2) * A[i] + A[i-1]) - z[i-1]) / l[i];
		}
		C[KNOTS-1] = 0;
		for (int i = KNOTS - 2; i >= 0; --i) {
			C[i] = z[i] - u[i] * C[i+1];
			B[i] = A[i+1] - A[i] - (C[i+1] + ITYPE(2) * C[i]) / ITYPE(3);
			D[i] = (C[i+1] - C[i]) / ITYPE(3);
		}
	}
	OTYPE operator () (ITYPE x)
	{
		int k = x;
		ITYPE t = x - ITYPE(k);
		if (k < 0) {
			t = x;
			k = 0;
		}
		if (k >= KNOTS - 1) {
			t = x - ITYPE(KNOTS-2);
			k = KNOTS-2;
		}
		return A[k] + t * (B[k] + t * (C[k] + t * D[k]));
	}
};

}

#endif

