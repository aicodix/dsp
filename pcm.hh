/*
Interface for reading and writing PCM data

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef PCM_HH
#define PCM_HH

namespace DSP {

template <typename TYPE>
struct WritePCM
{
	virtual void write(TYPE *, int, int) = 0;
	virtual void silence(int) = 0;
};

template <typename TYPE>
struct ReadPCM
{
	virtual void read(TYPE *, int, int) = 0;
};

}

#endif

