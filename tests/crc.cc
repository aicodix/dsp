/*
Test for the Cyclic redundancy check

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include <cassert>
#include <iostream>
#include "crc.hh"

int main()
{
	DSP::CRC<uint32_t> crc(0xEDB88320, 0xFFFFFFFF);
	for (uint8_t c: std::string("Hello World!")) crc(c);
	assert(!crc(uint32_t(~0x1C291CA3)));
	std::cerr << "Cyclic redundancy check test passed!" << std::endl;
	return 0;
}

