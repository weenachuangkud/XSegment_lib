# Makefile for XSegment_lib unit tests
# Usage: make test (or just make)

CC = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I../src -I../src/core -I../src/fonts -Ithird_party/unity

.PHONY: all test clean

all: math buffer

math: test_XSeg_Math
	./test_XSeg_Math

buffer: test_XSeg_Buffer
	./test_XSeg_Buffer

test_XSeg_Math: test_XSeg_Math.cpp third_party/unity/unity.c
	$(CC) $(CXXFLAGS) -o $@ test_XSeg_Math.cpp third_party/unity/unity.c

test_XSeg_Buffer: test_XSeg_Buffer.cpp third_party/unity/unity.c
	$(CC) $(CXXFLAGS) -o $@ test_XSeg_Buffer.cpp third_party/unity/unity.c

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