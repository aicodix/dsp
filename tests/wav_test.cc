/*
Test for the WAV reader and writer

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <cstdio>
#include <cmath>
#include <iostream>
#include "wav.hh"

void round_trip(int bits, int channels)
{
	const char *name = "wav_test.wav";
	const int rate = 8000, frames = 100;
	float out[frames * 2];
	for (int i = 0; i < frames * channels; ++i)
		out[i] = std::sin(float(i)) * 0.9f;
	{
		DSP::WriteWAV<float> wav(name, rate, bits, channels);
		wav.write(out, frames);
	}
	DSP::ReadWAV<float> wav(name);
	assert(wav.good());
	assert(wav.rate() == rate);
	assert(wav.bits() == bits);
	assert(wav.channels() == channels);
	assert(wav.frames() == frames);
	float inp[frames * 2];
	wav.read(inp, frames);
	assert(wav.good());
	float tolerance = bits == 32 ? 0 : bits == 16 ? 1.f / 32767 : 1.f / 127;
	for (int i = 0; i < frames * channels; ++i)
		assert(std::abs(inp[i] - out[i]) <= tolerance);
	std::remove(name);
}

void invalid(const char *name)
{
	DSP::ReadWAV<float> wav(name);
	assert(wav.rate() == 0);
	assert(wav.bits() == 0);
	assert(wav.channels() == 0);
	assert(wav.frames() == -1);
}

int main()
{
	for (int bits = 8; bits <= 32; bits *= 2)
		for (int channels = 1; channels <= 2; ++channels)
			round_trip(bits, channels);
	invalid("wav_test.cc");
	invalid("/dev/null");
	invalid("does_not_exist.wav");
	std::cerr << "WAV reader and writer test passed!" << std::endl;
	return 0;
}
