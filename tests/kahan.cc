/*
Test for the Kahan summation algorithm

Copyright 2018 Ahmet Inan <inan@aicodix.de>
*/

#include "kahan.hh"

int main()
{
	const double pi = 3.14159265358979323846;
	DSP::Kahan<double> kahan(3.0);
	for (double i = 2.0; i < 181423.0; i += 4.0) {
		kahan(4.0 / (i * (i + 1.0) * (i + 2.0)));
		kahan(-4.0 / ((i + 2.0) * (i + 3.0) * (i + 4.0)));
	}
	return kahan() != pi;
}

