/*
Some trigger functions

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace DSP {

template <typename TYPE>
class SchmittTrigger
{
	TYPE threshold;
	bool previous;
public:
	constexpr SchmittTrigger(TYPE threshold = TYPE(1)/TYPE(3), bool previous = false) : threshold(threshold), previous(previous)
	{
	}
	bool operator() (TYPE input)
	{
		if (previous) {
			if (input < -threshold)
				previous = false;
		} else {
			if (input > threshold)
				previous = true;
		}
		return previous;
	}
};

class FallingEdgeTrigger
{
	bool previous;
public:
	constexpr FallingEdgeTrigger(bool previous = false) : previous(previous)
	{
	}
	bool operator() (bool input)
	{
		bool tmp = previous;
		previous = input;
		return tmp && !input;
	}
};

class RisingEdgeTrigger
{
	bool previous;
public:
	constexpr RisingEdgeTrigger(bool previous = false) : previous(previous)
	{
	}
	bool operator() (bool input)
	{
		bool tmp = previous;
		previous = input;
		return !tmp && input;
	}
};

}

