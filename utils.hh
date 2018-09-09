/*
Some little helpers

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef UTILS_HH
#define UTILS_HH

namespace DSP {

template <typename TYPE>
int signum(TYPE v)
{
	return (v > TYPE(0)) - (v < TYPE(0));
}

}

#endif

