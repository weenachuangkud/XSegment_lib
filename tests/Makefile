# ─────────────────────────────────────────
#  XSegment_lib Unit Tests
#  Run on desktop — no hardware needed
#  Usage:
#    make        — build and run all
#    make math   — run math tests only
#    make font   — run font tests only
#    make buffer — run buffer tests only
#    make clean  — remove build files
# ─────────────────────────────────────────

CXX      = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I..

all: math font buffer
	@echo ""
	@echo "================================"
	@echo " All XSegment_lib Tests Passed"
	@echo "================================"

math: test_XSeg_Math
	./test_XSeg_Math

font: test_XSeg_Font
	./test_XSeg_Font

buffer: test_XSeg_Buffer
	./test_XSeg_Buffer

test_XSeg_Math: test_XSeg_Math.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_XSeg_Font: test_XSeg_Font.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test_XSeg_Buffer: test_XSeg_Buffer.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm -f test_XSeg_Math test_XSeg_Font test_XSeg_Buffer
