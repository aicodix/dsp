/*
Read and write WAV files

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef WAV_HH
#define WAV_HH

#include <fstream>
#include "pcm.hh"

namespace DSP {

template <typename TYPE>
class WriteWAV : public WritePCM<TYPE>
{
	std::ofstream os;
	int bytes, channels;
	int offset, factor, min, max;
	void writeLE(int v, int b)
	{
		for (int i = 0; i < b; ++i)
			os.put(255 & (v >> (8 * i)));
	}
public:
	WriteWAV(const char *name, int rate, int bits, int channels) : os(name, std::ios::binary | std::ios::trunc), bytes(bits / 8), channels(channels)
	{
		switch (bits) {
			case 8:
				offset = 128;
				factor = 127;
				min = 0;
				max = 255;
				break;
			default:
				bits = 16;
				bytes = 2;
				offset = 0;
				factor = 32767;
				min = -32768;
				max = 32767;
		}
		os.write("RIFF", 4); // ChunkID
		writeLE(36, 4); // ChunkSize
		os.write("WAVE", 4); // Format
		os.write("fmt ", 4); // Subchunk1ID
		writeLE(16, 4); // Subchunk1Size
		writeLE(1, 2); // AudioFormat
		writeLE(channels, 2); // NumChannels
		writeLE(rate, 4); // SampleRate
		writeLE(rate * channels * bytes, 4); // ByteRate
		writeLE(channels * bytes, 2); // BlockAlign
		writeLE(8 * bytes, 2); // BitsPerSample
		os.write("data", 4); // Subchunk2ID
		writeLE(0, 4); // Subchunk2Size
	}
	~WriteWAV()
	{
		int size = int(os.tellp()) - 44;
		os.seekp(4);
		writeLE(36 + size, 4); // ChunkSize
		os.seekp(40);
		writeLE(size, 4); // Subchunk2Size
	}
	void write(TYPE *buf, int num, int stride = 1)
	{
		for (int n = 0; n < num; ++n) {
			for (int c = 0; c < channels; ++c) {
				TYPE v = TYPE(offset) + TYPE(factor) * buf[stride * n + c];
				writeLE(std::nearbyint(std::clamp(v, TYPE(min), TYPE(max))), bytes);
			}
		}
	}
	void silence(int num)
	{
		for (int i = 0; i < num * channels; ++i)
			writeLE(offset, bytes);
	}
};

}

#endif

