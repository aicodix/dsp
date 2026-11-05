/*
sin and cos approximations

Constants below lifted from the Cephes Mathematical Library:
https://www.netlib.org/cephes/cmath.tgz

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "const.hh"

namespace DSP {

template <typename TYPE>
TYPE sin_kernel(TYPE x)
{
	static constexpr TYPE
		S0 = 1.58962301576546568060E-10,
		S1 = -2.50507477628578072866E-8,
		S2 = 2.75573136213857245213E-6,
		S3 = -1.98412698295895385996E-4,
		S4 = 8.33333333332211858878E-3,
		S5 = -1.66666666666666307295E-1;
	TYPE xx = x * x;
	return x + x * xx * (xx * (xx * (xx * (xx * (xx * S0 + S1) + S2) + S3) + S4) + S5);
}

template <typename TYPE>
TYPE cos_kernel(TYPE x)
{
	static constexpr TYPE
		C0 = -1.13585365213876817300E-11,
		C1 = 2.08757008419747316778E-9,
		C2 = -2.75573141792967388112E-7,
		C3 = 2.48015872888517045348E-5,
		C4 = -1.38888888888730564116E-3,
		C5 = 4.16666666666665929218E-2;
	TYPE xx = x * x;
	return TYPE(1) - TYPE(0.5) * xx + xx * xx * (xx * (xx * (xx * (xx * (xx * C0 + C1) + C2) + C3) + C4) + C5);
}

// reduce x to [-pi/4, pi/4] by the nearest multiple q of pi/2 and return q modulo 4
template <typename TYPE>
int sincos_quadrant(TYPE &x)
{
	TYPE q = nearbyint(x / Const<TYPE>::HalfPi());
	x -= q * Const<TYPE>::HalfPi();
	return int(q) & 3;
}

template <typename TYPE>
TYPE sin(TYPE x)
{
	switch (sincos_quadrant(x)) {
	case 1:
		return cos_kernel(x);
	case 2:
		return -sin_kernel(x);
	case 3:
		return -cos_kernel(x);
	}
	return sin_kernel(x);
}

template <typename TYPE>
TYPE cos(TYPE x)
{
	switch (sincos_quadrant(x)) {
	case 1:
		return -sin_kernel(x);
	case 2:
		return -cos_kernel(x);
	case 3:
		return sin_kernel(x);
	}
	return cos_kernel(x);
}

}

