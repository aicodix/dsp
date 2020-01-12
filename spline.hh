/*
Some spline algorithms

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace DSP {

template <int KNOTS, typename OTYPE, typename ITYPE>
class UniformNaturalCubicSpline
{
	OTYPE A[KNOTS-1], B[KNOTS-1], C[KNOTS-1], D[KNOTS-1];
	ITYPE x0, dx;
public:
	UniformNaturalCubicSpline() = default;
	UniformNaturalCubicSpline(OTYPE *y, ITYPE x0 = 0, ITYPE dx = 1, int STRIDE = 1) : x0(x0), dx(dx)
	{
		ITYPE u[KNOTS-1];
		u[0] = ITYPE(0);
		OTYPE z[KNOTS-1];
		z[0] = ITYPE(0);
		for (int i = 1; i < KNOTS - 1; ++i) {
			ITYPE l = ITYPE(4) - u[i-1];
			u[i] = ITYPE(1) / l;
			z[i] = (ITYPE(3) * (y[(i+1)*STRIDE] - ITYPE(2) * y[i*STRIDE] + y[(i-1)*STRIDE]) - z[i-1]) / l;
		}
		OTYPE c(ITYPE(0));
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

template <typename TYPE>
struct CubicHermiteSpline
{
	static constexpr TYPE h00(TYPE t)
	{
		return (TYPE(1) + TYPE(2) * t) * (TYPE(1) - t) * (TYPE(1) - t);
	}
	static constexpr TYPE h10(TYPE t)
	{
		return t * (TYPE(1) - t) * (TYPE(1) - t);
	}
	static constexpr TYPE h01(TYPE t)
	{
		return t * t * (TYPE(3) - TYPE(2) * t);
	}
	static constexpr TYPE h11(TYPE t)
	{
		return t * t * (t - TYPE(1));
	}
	static constexpr TYPE left(const TYPE *x, const TYPE *y)
	{
		return (y[0] - y[-1]) / (x[0] - x[-1]);
	}
	static constexpr TYPE right(const TYPE *x, const TYPE *y)
	{
		return (y[1] - y[0]) / (x[1] - x[0]);
	}
	static constexpr TYPE central(const TYPE *x, const TYPE *y)
	{
		return TYPE(0.5) * (left(x, y) + right(x, y));
	}
	static constexpr TYPE eval(const TYPE *x, const TYPE *y, TYPE t, int k, int n)
	{
		return k < 1 ?
			h00(t) * y[0] + h10(t) * (x[1]-x[0]) * right(x, y) + h01(t) * y[1] + h11(t) * (x[1]-x[0]) * central(x+1, y+1)
		: k < n-2 ?
			h00(t) * y[k] + h10(t) * (x[k+1]-x[k]) * central(x+k, y+k) + h01(t) * y[k+1] + h11(t) * (x[k+1]-x[k]) * central(x+k+1, y+k+1)
		:
			h00(t) * y[n-2] + h10(t) * (x[n-1]-x[n-2]) * central(x+n-2, y+n-2) + h01(t) * y[n-1] + h11(t) * (x[n-1]-x[n-2]) * left(x+n-1, y+n-1);
	}
	static constexpr TYPE left(const TYPE *y)
	{
		return y[0] - y[-1];
	}
	static constexpr TYPE right(const TYPE *y)
	{
		return y[1] - y[0];
	}
	static constexpr TYPE central(const TYPE *y)
	{
		return TYPE(0.5) * (y[1] - y[-1]);
	}
	static constexpr TYPE eval(const TYPE *y, TYPE t, int k, int n)
	{
		return k < 1 ?
			h00(t) * y[0] + h10(t) * right(y) + h01(t) * y[1] + h11(t) * central(y+1)
		: k < n-2 ?
			h00(t) * y[k] + h10(t) * central(y+k) + h01(t) * y[k+1] + h11(t) * central(y+k+1)
		:
			h00(t) * y[n-2] + h10(t) * central(y+n-2) + h01(t) * y[n-1] + h11(t) * left(y+n-1);
	}
};

}

