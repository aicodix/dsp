/*
Some trigger functions

Copyright 2019 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

namespace DSP {

template <typename TYPE>
class SchmittTrigger
{
	TYPE treshold;
	bool previous;
public:
	constexpr SchmittTrigger(TYPE treshold, bool previous = false) : treshold(treshold), previous(previous)
	{
	}
	bool operator() (TYPE input)
	{
		if (previous) {
			if (input < -treshold)
				previous = false;
		} else {
			if (input > treshold)
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

