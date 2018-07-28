/*
Cyclic redundancy check

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#ifndef CRC_HH
#define CRC_HH

namespace DSP {

template <typename TYPE>
class CRC
{
	static const int BYTES = sizeof(TYPE);
	TYPE lut[BYTES * 256];
	TYPE poly;
	TYPE crc;
public:
	CRC(TYPE poly, TYPE crc = 0) : poly(poly), crc(crc)
	{
		// exploit linearity of CRC only bytewise
		for (int k = 0; k < BYTES; ++k) {
			for (int j = 0; j < 256; ++j) {
				TYPE tmp = j << (k << 3);
				for (int i = 8; i; --i)
					tmp = (tmp >> 1) ^ ((tmp & 1) * poly);
				lut[(k << 8) + j] = tmp;
			}
		}
	}
	void reset(TYPE v)
	{
		crc = v;
	}
	TYPE operator()(bool data)
	{
		TYPE tmp = crc ^ data;
		tmp = (tmp >> 1) ^ ((tmp & 1) * poly);
		return crc = tmp;
	}
	TYPE operator()(uint8_t data)
	{
		TYPE tmp = crc ^ data;
		crc = lut[tmp & 255];
		for (int k = 1; k < BYTES; ++k)
			crc ^= lut[(k << 8) + ((tmp >>= 8) & 255)];
		return crc;
	}
	template <typename DATA>
	TYPE operator()(DATA data)
	{
		for (int i = sizeof(data); i; --i) {
			(*this)((uint8_t)data);
			data >>= 8;
		}
		return crc;
	}
};

template<>
uint16_t CRC<uint16_t>::operator()(uint8_t data)
{
	uint16_t tmp = crc ^ data;
	return crc = lut[256 + (tmp >> 8)] ^ lut[tmp & 255];
}

template<>
uint8_t CRC<uint8_t>::operator()(uint8_t data)
{
	return crc = lut[crc ^ data];
}

}

#endif

