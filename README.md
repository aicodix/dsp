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

### [filter.hh](filter.hh)

Implemented are the following [finite impulse response](https://en.wikipedia.org/wiki/Finite_impulse_response) [filters](https://en.wikipedia.org/wiki/Filter_(signal_processing)):
* [low-pass filter](https://en.wikipedia.org/wiki/Low-pass_filter)
* [high-pass filter](https://en.wikipedia.org/wiki/High-pass_filter)
* [band-pass filter](https://en.wikipedia.org/wiki/Band-pass_filter)

### [ema.hh](ema.hh)

The [exponential moving average](https://en.wikipedia.org/wiki/Moving_average#Exponential_moving_average) is an [infinite impulse response](https://en.wikipedia.org/wiki/Infinite_impulse_response) [low-pass filter](https://en.wikipedia.org/wiki/Low-pass_filter).
There is also support for cascading, to improve [roll-off](https://en.wikipedia.org/wiki/Roll-off) while a correction factor helps to keep the same [cutoff frequency](https://en.wikipedia.org/wiki/Cutoff_frequency).

### [biquad.hh](biquad.hh)

The following [infinite impulse response](https://en.wikipedia.org/wiki/Infinite_impulse_response) [digital biquad filter](https://en.wikipedia.org/wiki/Digital_biquad_filter) implementations are available:

* [second-order Butterworth low pass filter](https://en.wikipedia.org/wiki/Butterworth_filter)
* [2n-order Butterworth cascade of second-order low pass filters](https://en.wikipedia.org/wiki/Butterworth_filter)

### [phasor.hh](phasor.hh)

[Numerically controlled oscillator](https://en.wikipedia.org/wiki/Numerically_controlled_oscillator) implemented using a [phasor](https://en.wikipedia.org/wiki/Phasor) and [complex multiplication](https://en.wikipedia.org/wiki/Complex_number#Multiplication) instead of a [lookup table](https://en.wikipedia.org/wiki/Lookup_table).

### [fmd.hh](fmd.hh)

[Frequency modulation](https://en.wikipedia.org/wiki/Frequency_modulation) [demodulation](https://en.wikipedia.org/wiki/Demodulation) with and without [atan2](https://en.wikipedia.org/wiki/Atan2).

### [amd.hh](amd.hh)

[Amplitude modulation](https://en.wikipedia.org/wiki/Amplitude_modulation) [demodulation](https://en.wikipedia.org/wiki/Demodulation) with [automatic gain control](https://en.wikipedia.org/wiki/Automatic_gain_control).

### [atan2.hh](atan2.hh)

[atan](https://en.wikipedia.org/wiki/Inverse_trigonometric_functions) and [atan2](https://en.wikipedia.org/wiki/Atan2).

### [const.hh](const.hh)

Some constants we need

### [pcm.hh](pcm.hh)

Interface for reading and writing [PCM](https://en.wikipedia.org/wiki/Pulse-code_modulation) data

### [wav.hh](wav.hh)

Read and write [WAV](https://en.wikipedia.org/wiki/WAV) files

### [spline.hh](spline.hh)

Algorithm for computing uniform and [natural cubic splines](https://en.wikipedia.org/wiki/Spline_(mathematics)#Algorithm_for_computing_natural_cubic_splines)
Very useful for data interpolation.

### [regression.hh](regression.hh)

Implemented [Simple linear regression](https://en.wikipedia.org/wiki/Simple_linear_regression) for [Regression analysis](https://en.wikipedia.org/wiki/Regression_analysis) of data.

### [complex.hh](complex.hh)

Faster alternative (no Inf/NaN handling) to the std::complex implementation.

### [fft.hh](fft.hh)

Mixed-radix [decimation-in-time](https://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm) [fast Fourier transform](https://en.wikipedia.org/wiki/Fast_Fourier_transform)

### [utils.hh](utils.hh)

Some everyday helpers:
* [signum function](https://en.wikipedia.org/wiki/Sign_function)
* [lerp function](https://en.wikipedia.org/wiki/Linear_interpolation)
* [probability density function](https://en.wikipedia.org/wiki/Probability_density_function) of the [normal distribution](https://en.wikipedia.org/wiki/Normal_distribution)
* [sinc function](https://en.wikipedia.org/wiki/Sinc_function)
* [delta function](https://en.wikipedia.org/wiki/Dirac_delta_function)
* [decibel function](https://en.wikipedia.org/wiki/Decibel)

### [resampler.hh](resampler.hh)

When working with [Analog-to-digital](https://en.wikipedia.org/wiki/Analog-to-digital_converter) and [Digital-to-analog](https://en.wikipedia.org/wiki/Digital-to-analog_converter) converters, we often face the ugly truth, that we can't always have a precise [Sampling](https://en.wikipedia.org/wiki/Sampling_(signal_processing)) rate.
But if we can estimate the Sampling frequency offset, we can correct it by [Resampling](https://en.wikipedia.org/wiki/Sample-rate_conversion) the sampled data.

### [unit_circle.hh](unit_circle.hh)

Sometimes we only need [trigonometric functions](https://en.wikipedia.org/wiki/Trigonometric_functions) that stay on the [unit circle](https://en.wikipedia.org/wiki/Unit_circle):
* [sine function](https://en.wikipedia.org/wiki/Sine)
* [cosine function](https://en.wikipedia.org/wiki/Trigonometric_functions#cosine)

