/*
Some constants

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef CONST_HH
#define CONST_HH

namespace DSP {

template <typename T>
struct Const {
	static constexpr T EighthPi()	{ return   0.39269908169872415481; } // a(1)/2
	static constexpr T FourthPi()	{ return   0.78539816339744830962; } // 1*a(1)
	static constexpr T HalfPi()	{ return   1.57079632679489661923; } // 2*a(1)
	static constexpr T Pi()		{ return   3.14159265358979323846; } // 4*a(1)
	static constexpr T TwoPi()	{ return   6.28318530717958647693; } // 8*a(1)
	static constexpr T FourPi()	{ return  12.56637061435917295385; } // 16*a(1)
	static constexpr T SqrtPi()	{ return   1.77245385090551602730; } // sqrt(4*a(1))
	static constexpr T SqrtTwoPi()	{ return   2.50662827463100050242; } // sqrt(8*a(1))
};

}

#endif

