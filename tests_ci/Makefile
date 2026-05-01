# Makefile for XSegment_lib unit tests
# Usage: make test (or just make)

CC = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I../src -I../src/core -I../src/fonts

.PHONY: all test clean

all: math buffer

math: test_XSeg_Math
	./test_XSeg_Math

buffer: test_XSeg_Buffer
	./test_XSeg_Buffer

test_XSeg_Math: math/test_XSeg_Math.cpp src/fonts/XSeg_Font.c
	$(CC) $(CXXFLAGS) -o $@ math/test_XSeg_Math.cpp src/fonts/XSeg_Font.c -lunity

test_XSeg_Buffer: buffer/test_XSeg_Buffer.cpp src/fonts/XSeg_Font.c
	$(CC) $(CXXFLAGS) -o $@ buffer/test_XSeg_Buffer.cpp src/fonts/XSeg_Font.c -lunity

test: all
	@echo ""
	@echo "================================"
	@echo " All XSegment_lib Tests Passed"
	@echo "================================"

clean:
	rm -f test_XSeg_Math test_XSeg_Buffer

help:
	@echo "Available targets:"
	@echo "  make test       - Run all unit tests"
	@echo "  make math      - Run math tests only"
	@echo "  make buffer    - Run buffer tests only"
	@echo "  make clean     - Remove compiled binaries"