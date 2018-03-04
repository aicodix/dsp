/*
Some constants

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef CONST_HH
#define CONST_HH

namespace DSP {

template <typename T>
struct Const {
	static inline T Pi()	{ return 3.14159265358979323846; } // 4*a(1)
	static inline T TwoPi()	{ return 6.28318530717958647693; } // 8*a(1)
};

}

#endif

