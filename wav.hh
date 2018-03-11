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
class ReadWAV : public ReadPCM<TYPE>
{
	std::ifstream is;
	int bits, bytes, rate, channels;
	int offset, factor;
	int readLE(int b)
	{
		int v = 0;
		for (int i = 0; i < b; ++i)
			v |= is.get() << (8 * i);
		if (b == 2 && v > 32767)
			v |= ~32767;
		if (b == 3 && v > 8388607)
			v |= ~8388607;
		return v;
	}
	bool cmp4(const char *a, const char *b)
	{
		for (int i = 0; i < 4; ++i)
			if (a[i] != b[i])
				return true;
		return false;
	}
public:
	ReadWAV(const char *name) : is(name, std::ios::binary)
	{
		char ChunkID[4];
		is.read(ChunkID, 4);
		if (cmp4("RIFF", ChunkID))
			return;
		int ChunkSize = readLE(4);
		char Format[4];
		is.read(Format, 4);
		if (cmp4("WAVE", Format))
			return;
		char Subchunk1ID[4];
		is.read(Subchunk1ID, 4);
		if (cmp4("fmt ", Subchunk1ID))
			return;
		int Subchunk1Size = readLE(4);
		if (Subchunk1Size != 16)
			return;
		int AudioFormat = readLE(2);
		if (AudioFormat != 1)
			return;
		channels = readLE(2);
		rate = readLE(4);
		int ByteRate = readLE(4);
		int BlockAlign = readLE(2);
		bits = readLE(2);
		if (bits != 8 && bits != 16 && bits != 24 && bits != 32)
			return;
		bytes = bits / 8;
		if (bytes * channels != BlockAlign)
			return;
		if (rate * bytes * channels != ByteRate)
			return;
		char Subchunk2ID[4];
		is.read(Subchunk2ID, 4);
		if (cmp4("data", Subchunk2ID))
			return;
		int Subchunk2Size = readLE(4);
		if (36 + Subchunk2Size != ChunkSize)
			return;

		switch (bits) {
			case 8:
				offset = 128;
				factor = 127;
				break;
			case 16:
				offset = 0;
				factor = 32767;
				break;
			case 24:
				offset = 0;
				factor = 8388607;
				break;
			case 32:
				offset = 0;
				factor = 2147483647;
				break;
			default:
				return;
		}
	}
	void read(TYPE *buf, int num, int stride = 1)
	{
		for (int n = 0; n < num; ++n) {
			for (int c = 0; c < channels; ++c) {
				buf[stride * n + c] = TYPE(readLE(bytes) - offset) / TYPE(factor);
			}
		}
	}
};

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
			case 16:
				offset = 0;
				factor = 32767;
				min = -32768;
				max = 32767;
				break;
			case 24:
				offset = 0;
				factor = 8388607;
				min = -8388608;
				max = 8388607;
				break;
			case 32:
				offset = 0;
				factor = 2147483647;
				min = -2147483648;
				max = 2147483647;
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

