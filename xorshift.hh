/*
Class of pseudorandom number generators, discovered by George Marsaglia

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef XORSHIFT_HH
#define XORSHIFT_HH

namespace DSP {

class Xorshift32
{
	uint32_t y;
public:
	Xorshift32(uint32_t y = 2463534242) : y(y) {}
	uint32_t operator()()
	{
		y ^= y << 13;
		y ^= y >> 17;
		y ^= y << 5;
		return y;
	}
};

class Xorshift64
{
	uint64_t x;
public:
	Xorshift64(uint64_t x = 88172645463325252) : x(x) {}
	uint64_t operator()()
	{
		x ^= x << 13;
		x ^= x >> 7;
		x ^= x << 17;
		return x;
	}
};

class Xorwow
{
	uint32_t x, y, z, w, v, d;
public:
	Xorwow(uint32_t x = 123456789, uint32_t y = 362436069,
		uint32_t z = 521288629, uint32_t w = 88675123,
		uint32_t v = 5783321, uint32_t d = 6615241) :
		x(x), y(y), z(z), w(w), v(v), d(d) {}
	uint32_t operator()()
	{
		uint32_t t = x ^ (x >> 2);
		x = y; y = z; z = w; w = v;
		v = (v ^ (v << 4)) ^ (t ^ (t << 1));
		d += 362437;
		return d + v;
	}
};

class Xorshift128
{
	uint32_t x, y, z, w;
public:
	Xorshift128(uint32_t x = 123456789, uint32_t y = 362436069,
		uint32_t z = 521288629, uint32_t w = 88675123) :
		x(x), y(y), z(z), w(w) {}
	uint32_t operator()()
	{
		uint32_t t = (x ^ (x << 11));
		x = y; y = z; z = w;
		w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
		return w;
	}
};

}

#endif

