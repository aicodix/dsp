This is a work in progress and a long overdue attempt to bring all our DSP code together and make it reusable for our future projects.

Before using any of this you should enter the tests directory and execute "make".
This will check if your compiler is able to create binaries that are able to produce correct results when executed.

What we have included so far:

### [kahan.hh](kahan.hh)

When working with [Floating-point arithmetic](https://en.wikipedia.org/wiki/Floating-point_arithmetic) we soon realize, that addition is not necessarily [associative](https://en.wikipedia.org/wiki/Associative_property).
Whenever we need to add numbers with an ever decreasing magnitude to an sum (of the previous numbers) with an ever increasing magnitude, the [Kahan summation algorithm](https://en.wikipedia.org/wiki/Kahan_summation_algorithm) comes in handy and helps keeping the error growth small.

