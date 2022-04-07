
CXXFLAGS = -I.. -std=c++11 -W -Wall -O2 -fno-exceptions -fno-rtti -ffast-math -ftree-vectorize -march=native
CXX = clang++ -stdlib=libc++
#CXX = g++

.PHONY: clean test

tests := $(basename $(wildcard *_test.*))

test: $(tests)
	$(patsubst %,./%;,$(tests))

clean:
	rm -f *_test

