/*
Moving Extrema

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

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
	struct Equal { bool operator () (TYPE a, TYPE b) { return a == b; } };
	struct Less { bool operator () (TYPE a, TYPE b) { return a < b; } };
	MovExt<TYPE, Equal, Less, NUM> movmin;
public:
	TYPE operator () (TYPE input)
	{
		return movmin(input);
	}
};

template <typename TYPE, int NUM>
class MovMax
{
	struct Equal { bool operator () (TYPE a, TYPE b) { return a == b; } };
	struct Greater { bool operator () (TYPE a, TYPE b) { return a > b; } };
	MovExt<TYPE, Equal, Greater, NUM> movmax;
public:
	TYPE operator () (TYPE input)
	{
		return movmax(input);
	}
};

}

