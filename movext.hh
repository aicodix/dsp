/*
Moving Extrema

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#pragma once

#include "deque.hh"

namespace DSP {

template <typename TYPE, int NUM>
class MovMin
{
	Deque<TYPE, NUM> window, dispenser, refill;
public:
	TYPE operator () (TYPE input)
	{
		if (window.full()) {
			if (window.front() == dispenser.front())
				dispenser.pop_front();
			window.pop_front();
		}
		window.push_back(input);

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
	Deque<TYPE, NUM> window, dispenser, refill;
public:
	TYPE operator () (TYPE input)
	{
		if (window.full()) {
			if (window.front() == dispenser.front())
				dispenser.pop_front();
			window.pop_front();
		}
		window.push_back(input);

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

