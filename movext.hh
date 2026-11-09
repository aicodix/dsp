/*
Moving Extrema

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "delay.hh"

namespace DSP {

// the extremum of a window kept elsewhere, with the dispenser and refill stacks in one array,
// as they never hold more than NUM values together:
// the dispenser grows up from the bottom and the refill down from the top
template <typename TYPE, typename EQUAL, typename COMP, int NUM>
class MovExtStack
{
	TYPE stack[NUM];
	int disp, fill;
	EQUAL equal;
	COMP comp;
public:
	MovExtStack() : disp(0), fill(NUM)
	{
	}
	MovExtStack(TYPE init) : disp(1), fill(NUM)
	{
		stack[0] = init;
	}
	// takes the value leaving the window, once the window is full
	void remove(TYPE output)
	{
		if (equal(output, stack[disp-1]))
			--disp;
	}
	// takes the value entering the window and returns the extremum
	TYPE insert(TYPE input)
	{
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

template <typename TYPE, typename EQUAL, typename COMP, int NUM>
class MovExt
{
	Delay<TYPE, NUM> window;
	MovExtStack<TYPE, EQUAL, COMP, NUM> ext;
public:
	MovExt(TYPE init) : window(init), ext(init)
	{
	}
	TYPE operator () (TYPE input)
	{
		ext.remove(window(input));
		return ext.insert(input);
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

// both from one window, which can't start with a value that is the worst for both,
// so the extrema only take the inputs and see no values leave until NUM came in
template <typename TYPE, int NUM>
class MovMinMax
{
	Delay<TYPE, NUM> window;
	MovExtStack<TYPE, std::equal_to<TYPE>, std::less<TYPE>, NUM> movmin;
	MovExtStack<TYPE, std::equal_to<TYPE>, std::greater<TYPE>, NUM> movmax;
	TYPE min_value, max_value;
	int count;
public:
	MovMinMax() : count(0)
	{
	}
	void operator () (TYPE input)
	{
		TYPE output = window(input);
		if (count < NUM) {
			++count;
		} else {
			movmin.remove(output);
			movmax.remove(output);
		}
		min_value = movmin.insert(input);
		max_value = movmax.insert(input);
	}
	TYPE minimum()
	{
		return min_value;
	}
	TYPE maximum()
	{
		return max_value;
	}
};

}
