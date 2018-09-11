This is a work in progress and a long overdue attempt to bring all our DSP code together and make it reusable for our future projects.

Before using any of this you should enter the tests directory and execute "make".
This will check if your compiler is able to create binaries that are able to produce correct results when executed.

What we have included so far:

### [kahan.hh](kahan.hh)

When working with [Floating-point arithmetic](https://en.wikipedia.org/wiki/Floating-point_arithmetic) we soon realize, that addition is not necessarily [associative](https://en.wikipedia.org/wiki/Associative_property).
For example, whenever we need to add values with an ever decreasing magnitude to a running sum with an ever increasing magnitude, the [Kahan summation algorithm](https://en.wikipedia.org/wiki/Kahan_summation_algorithm) comes in handy and helps keeping the error growth small.

### [window.hh](window.hh)

Implemented are the follwing [Window functions](https://en.wikipedia.org/wiki/Window_function):
* [Rectangular window](https://en.wikipedia.org/wiki/Window_function#Rectangular_window)
* [Hann window](https://en.wikipedia.org/wiki/Window_function#Hann_window)
* [Hamming window](https://en.wikipedia.org/wiki/Window_function#Hamming_window)
* [Lanczos window](https://en.wikipedia.org/wiki/Window_function#Lanczos_window)
* [Blackman window](https://en.wikipedia.org/wiki/Window_function#Blackman_window)
* [Gaussian window](https://en.wikipedia.org/wiki/Window_function#Gaussian_window)
* [Kaiser window](https://en.wikipedia.org/wiki/Window_function#Kaiser_window)

### [const.hh](const.hh)

Some constants we need

### [pcm.hh](pcm.hh)

Interface for reading and writing [PCM](https://en.wikipedia.org/wiki/Pulse-code_modulation) data

### [wav.hh](wav.hh)

Read and write [WAV](https://en.wikipedia.org/wiki/WAV) files

### [spline.hh](spline.hh)

Algorithm for computing uniform and [natural cubic splines](https://en.wikipedia.org/wiki/Spline_(mathematics)#Algorithm_for_computing_natural_cubic_splines)
Very useful for data interpolation.

### [crc.hh](crc.hh)

A [Cyclic redundancy check](https://en.wikipedia.org/wiki/Cyclic_redundancy_check) may not really be DSP-related, but it is needed over and over again when you do DSP that it fits just perfectly here.

For example, if we need to integrate CRC32 checking for a few bytes, like in the following:
```
# echo -n 'Hello World!' | rhash -C -
(stdin) 1C291CA3
```
We can add it to our project as simple as that:
```
DSP::CRC<uint32_t> crc(0xEDB88320, 0xFFFFFFFF);
for (uint8_t c: std::string("Hello World!")) crc(c);
assert(!crc(uint32_t(~0x1C291CA3)));
```

### [regression.hh](regression.hh)

Implemented [Simple linear regression](https://en.wikipedia.org/wiki/Simple_linear_regression) for [Regression analysis](https://en.wikipedia.org/wiki/Regression_analysis) of data.

### [complex.hh](complex.hh)

Faster alternative (no Inf/NaN handling) to the std::complex implementation.

### [fft.hh](fft.hh)

Mixed-radix [decimation-in-time](https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm) [fast Fourier transform](https://en.wikipedia.org/wiki/Fast_Fourier_transform)

### [utils.hh](utils.hh)

Some everyday helpers, like the [signum function](https://en.wikipedia.org/wiki/Sign_function) or the [lerp function](https://en.wikipedia.org/wiki/Linear_interpolation).

