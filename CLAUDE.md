# CLAUDE.md

Header-only C++17 library of DSP building blocks in namespace `DSP`: filters,
windows, oscillators, buffers, moving averages, estimators, FFT, WAV and Netpbm
I/O. README.md has one section per header.

Other aicodix projects, such as [modem](https://github.com/aicodix/modem) and
[disorders](https://github.com/aicodix/disorders), use it as a sibling checkout
found by `-I../dsp`. A change to an existing header can break them, so prefer
adding a new class or overload to changing what an existing one does. When a
change does alter an interface or its results, say which projects need
updating, and build those you have at hand.

## Building and testing

There is nothing to build in the top directory. The tests check that the
compiler produces binaries that give correct results:

```
cd tests
make
make clean
```

`make` builds every `*_test.cc` (picked up by wildcard) and runs it. A test is a
small `main()` that checks results with `assert()` and prints a "... test
passed!" line on stderr. Most headers have no test; add one when a header has
results that can be checked exactly or against a reference, such as `std::`
functions or a brute-force version.

The default compiler is `clang++ -stdlib=libc++ -march=native`. If libc++ is
not installed, run `make CXX="g++ -march=native"`. The Makefile has
commented-out lines for `g++` and for ARM cross-compilation under QEMU.

The tests use `-std=c++17 -O2 -ffast-math -ftree-vectorize -fno-exceptions
-fno-rtti`, and the projects using this library build with similar flags, so
code must:

- not use exceptions or RTTI;
- stay correct under `-ffast-math`. Don't rely on signed zeros, NaN or infinity
  checks, or a strict order of floating-point operations. `complex.hh` is a
  faster alternative to `std::complex` precisely because it skips Inf/NaN
  handling. Where order matters, use `Kahan` or a tree reduction (`SWA`, `SMA4`).

## Headers

- Headers include other headers of this library with quotes, but no standard
  headers, apart from `<fstream>` in `wav.hh` and `netpbm.hh`. Unqualified calls
  such as `abs`, `min`, `sqrt`, `sin`, `cos` or `exp` are resolved by the
  includer, for example with
  `namespace DSP { using std::abs; using std::min; using std::cos; using std::sin; }`
  before the includes, and by the overloads in `complex.hh` for `Complex`.
  Tests include the standard headers they need themselves.
- Sizes are template parameters (`NUM`, `TAPS`, `SIZE`) and storage is fixed-size
  member arrays. No heap allocation, no STL containers.
- Templates take the sample type `TYPE` and, where it differs, the scalar
  `VALUE`, so the same class works for real and complex samples, for example
  `BlockDC<TYPE, VALUE>` or `SMA4<cmplx, value, NUM>`.

## Interfaces

Keep new code in the shape of the existing classes:

- Processing objects are called with `operator()`: one sample in, one sample out,
  keeping their state between calls (`BlockDC`, `Delay`, `Hilbert`, `EMA`).
  Generators take no argument (`Phasor`).
- Buffers return a `const TYPE *` to contiguous data (`BipBuffer`).
- Filter kernels and windows are functors called as `(n, N)` for tap `n` of `N`
  (`LowPass`, `HighPass`, `Kaiser`, `Hann`), so they can be multiplied tap by
  tap, by hand or with `Coeffs`.
- Configuration that can change at run time goes into small setters
  (`BlockDC::samples()`, `Phasor::omega()`, `Phasor::freq()`).

## Code style

- Every file starts with a short C comment block: a one-line description,
  a blank line, then `Copyright <year> Ahmet Inan <inan@aicodix.de>`.
- Headers use `#pragma once`, then their includes, then `namespace DSP {` with a
  blank line on both sides of the contents.
- Indent with tabs and use K&R braces. Keep the code compact and avoid
  unnecessary abstraction.
- A new header gets a section in README.md: a link to the header and a sentence
  or two, with links to the relevant Wikipedia articles like the existing
  sections.

## Workflow

- Work on the `master` branch.
- Commit messages are a single short lowercase line, such as
  "added clamp()" or "made methods const". There is no body and no trailers:
  no `Co-Authored-By`, no session links.
- Don't push. Deliver changes as a `git format-patch` patch for the maintainer
  to review and apply.
- Check a change by running the tests, plus a dedicated test or a comparison
  against a reference where the change computes something new. A refactoring
  should give bit-identical results. Say how you checked it.
