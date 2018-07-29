
CXXFLAGS = -I.. -std=c++11 -W -Wall -O3 -march=native -ffast-math
CXX = clang++ -stdlib=libc++
#CXX = g++

.PHONY: clean test

test: kahan crc
	./kahan
	./crc

clean:
	rm -f kahan crc

