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
	OTYPE A[KNOTS-1], B[KNOTS-1], C[KNOTS-1], D[KNOTS-1];
	ITYPE x0, dx;
public:
	UniformNaturalCubicSpline(OTYPE *y, ITYPE x0 = 0, ITYPE dx = 1, int STRIDE = 1) : x0(x0), dx(dx)
	{
		ITYPE u[KNOTS-1];
		u[0] = 0;
		OTYPE z[KNOTS-1];
		z[0] = 0;
		for (int i = 1; i < KNOTS - 1; ++i) {
			ITYPE l = ITYPE(4) - u[i-1];
			u[i] = ITYPE(1) / l;
			z[i] = (ITYPE(3) * (y[(i+1)*STRIDE] - ITYPE(2) * y[i*STRIDE] + y[(i-1)*STRIDE]) - z[i-1]) / l;
		}
		OTYPE c = 0;
		for (int i = KNOTS - 2; i >= 0; --i) {
			A[i] = y[i * STRIDE];
			C[i] = z[i] - u[i] * c;
			B[i] = y[(i+1)*STRIDE] - y[i*STRIDE] - (c + ITYPE(2) * C[i]) / ITYPE(3);
			D[i] = (c - C[i]) / ITYPE(3);
			c = C[i];
		}
	}
	OTYPE operator () (ITYPE x)
	{
		ITYPE tx = (x - x0) / dx;
		int k = tx;
		ITYPE t = tx - ITYPE(k);
		if (k < 0) {
			t = tx;
			k = 0;
		}
		if (k >= KNOTS - 1) {
			t = tx - ITYPE(KNOTS-2);
			k = KNOTS-2;
		}
		return A[k] + t * (B[k] + t * (C[k] + t * D[k]));
	}
};

}

#endif

