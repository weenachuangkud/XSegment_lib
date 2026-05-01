# Makefile for running unit tests
# Usage: make test (or just make)

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -I src -I src/core -I src/fonts -I src/drivers -I src/platform
LDFLAGS = -lunity

# Find all test files
TEST_FILES = $(wildcard test_*.cpp)
TARGETS = $(TEST_FILES:.cpp=_test)

.PHONY: all test clean run_visual

all: test

test: $(TARGETS)
	@echo ""
	@echo "All tests passed!"

$(TARGETS): %_test: %.cpp
	@echo "Running $<..."
	@$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) && ./$@

clean:
	rm -f test_*_test test_*_test.exe

run_visual:
	@echo "Running visual simulator..."
	@$(CC) $(CFLAGS) src/visual/XSeg_VisualSim.cpp -o visual_test && ./visual_test

help:
	@echo "Available targets:"
	@echo "  make test        - Run all unit tests"
	@echo "  make run_visual  - Run console visual simulator"
	@echo "  make clean       - Remove compiled test binaries"
	@echo ""
	@echo "Run individual tests:"
	@echo "  make math_test   - Run XSeg_Math tests"
	@echo "  make buffer_test - Run XSeg_Buffer tests"