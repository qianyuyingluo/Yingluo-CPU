CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic

.PHONY: all test clean
all: build/yingluo
build:
	mkdir -p build
build/yingluo: src/main.cpp src/cpu.hpp | build
	$(CXX) $(CXXFLAGS) -Isrc src/main.cpp -o $@
build/cpu_test: tests/cpu_test.cpp src/cpu.hpp | build
	$(CXX) $(CXXFLAGS) -Isrc tests/cpu_test.cpp -o $@
test: build/cpu_test
	./build/cpu_test
clean:
	rm -rf build
