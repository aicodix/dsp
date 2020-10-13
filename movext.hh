/*
Moving Extrema

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "delay.hh"
#include "deque.hh"

namespace DSP {

template <typename TYPE, typename EQUAL, typename COMP, int NUM>
class MovExt
{
	Deque<TYPE, NUM> window, dispenser, refill;
	EQUAL equal;
	COMP comp;
public:
	TYPE operator () (TYPE input)
	{
		if (window.full()) {
			if (equal(window.front(), dispenser.front()))
				dispenser.pop_front();
			window.pop_front();
		}
		window.push_back(input);

		while (!refill.empty() && comp(input, refill.front()))
			refill.pop_front();
		refill.push_front(input);

		if (dispenser.empty()) {
			while (!refill.empty()) {
				dispenser.push_front(refill.front());
				refill.pop_front();
			}
			return dispenser.front();
		}
		return comp(dispenser.front(), refill.back()) ? dispenser.front() : refill.back();
	}
};

template <typename TYPE, int NUM>
class MovMin
{
	Delay<TYPE, NUM> window;
	Deque<TYPE, NUM> dispenser, refill;
public:
	MovMin() : window(std::numeric_limits<TYPE>::max())
	{
		dispenser.push_front(std::numeric_limits<TYPE>::max());
	}
	TYPE operator () (TYPE input)
	{
		if (window(input) == dispenser.front())
			dispenser.pop_front();

		while (!refill.empty() && input < refill.front())
			refill.pop_front();
		refill.push_front(input);

		if (dispenser.empty()) {
			while (!refill.empty()) {
				dispenser.push_front(refill.front());
				refill.pop_front();
			}
			return dispenser.front();
		}
		return dispenser.front() < refill.back() ? dispenser.front() : refill.back();
	}
};

template <typename TYPE, int NUM>
class MovMax
{
	Delay<TYPE, NUM> window;
	Deque<TYPE, NUM> dispenser, refill;
public:
	MovMax() : window(std::numeric_limits<TYPE>::min())
	{
		dispenser.push_front(std::numeric_limits<TYPE>::min());
	}
	TYPE operator () (TYPE input)
	{
		if (window(input) == dispenser.front())
			dispenser.pop_front();

		while (!refill.empty() && input > refill.front())
			refill.pop_front();
		refill.push_front(input);

		if (dispenser.empty()) {
			while (!refill.empty()) {
				dispenser.push_front(refill.front());
				refill.pop_front();
			}
			return dispenser.front();
		}
		return dispenser.front() > refill.back() ? dispenser.front() : refill.back();
	}
};

}

