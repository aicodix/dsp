/*
Moving Extrema

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "delay.hh"

namespace DSP {

template <typename TYPE, typename EQUAL, typename COMP, int NUM>
class MovExt
{
	Delay<TYPE, NUM> window;
	// the dispenser and refill stacks share one array, as they never hold more than NUM values together:
	// the dispenser grows up from the bottom and the refill down from the top
	TYPE stack[NUM];
	int disp, fill;
	EQUAL equal;
	COMP comp;
public:
	MovExt(TYPE init) : window(init), disp(1), fill(NUM)
	{
		stack[0] = init;
	}
	TYPE operator () (TYPE input)
	{
		if (equal(window(input), stack[disp-1]))
			--disp;

		while (fill < NUM && comp(input, stack[fill]))
			++fill;
		assert(disp < fill);
		stack[--fill] = input;

		// the oldest refill value becomes the top of the dispenser
		if (!disp) {
			TYPE top = stack[NUM-1];
			while (fill < NUM)
				stack[disp++] = stack[fill++];
			return top;
		}
		return comp(stack[disp-1], stack[NUM-1]) ? stack[disp-1] : stack[NUM-1];
	}
};

template <typename TYPE, int NUM>
class MovMin
{
	MovExt<TYPE, std::equal_to<TYPE>, std::less<TYPE>, NUM> movmin;
public:
	MovMin() : movmin(std::numeric_limits<TYPE>::max())
	{
	}
	TYPE operator () (TYPE input)
	{
		return movmin(input);
	}
};

template <typename TYPE, int NUM>
class MovMax
{
	MovExt<TYPE, std::equal_to<TYPE>, std::greater<TYPE>, NUM> movmax;
public:
	MovMax() : movmax(std::numeric_limits<TYPE>::lowest())
	{
	}
	TYPE operator () (TYPE input)
	{
		return movmax(input);
	}
};

}

