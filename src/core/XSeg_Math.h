#pragma once
#include <stdint.h>

#ifdef __AVR__
    #include <stdlib.h>   // abs() for AVR
#else
    #include <cstdlib>    // abs() for everything else
#endif

// ─────────────────────────────────────────
//  XSeg_Math.h
//  Pure math utilities for segment displays
//  No hardware, no dependencies
//  Zero overhead — all static inline
// ─────────────────────────────────────────

class XSeg_Math {
public:

    // ─────────────────────────────────────
    //  Fast Power of 10
    //  Lookup table for 0-9
    //  Loop fallback for 10-19
    //  uint64_t max ~18 digits
    // ─────────────────────────────────────

    static uint64_t fast10Pow(uint8_t exp) {
        switch (exp) {
            case 0:  return 1ULL;
            case 1:  return 10ULL;
            case 2:  return 100ULL;
            case 3:  return 1000ULL;
            case 4:  return 10000ULL;
            case 5:  return 100000ULL;
            case 6:  return 1000000ULL;
            case 7:  return 10000000ULL;
            case 8:  return 100000000ULL;
            case 9:  return 1000000000ULL;
            case 10: return 10000000000ULL;
            case 11: return 100000000000ULL;
            case 12: return 1000000000000ULL;
            case 13: return 10000000000000ULL;
            case 14: return 100000000000000ULL;
            case 15: return 1000000000000000ULL;
            case 16: return 10000000000000000ULL;
            case 17: return 100000000000000000ULL;
            case 18: return 1000000000000000000ULL;
            default: return 0ULL;  // out of range → 0 not 1
        }
    }


    // ─────────────────────────────────────
    //  Digit Extraction
    //  getDigitAt(1234, 0) → 4  (ones)
    //  getDigitAt(1234, 1) → 3  (tens)
    //  getDigitAt(1234, 2) → 2  (hundreds)
    //  getDigitAt(1234, 3) → 1  (thousands)
    // ─────────────────────────────────────

    static inline uint8_t getDigitAt(int32_t n, uint8_t pos) {
        return (uint8_t)((abs(n) / fast10Pow(pos)) % 10);
    }

    static inline uint8_t getDigitAt(uint32_t n, uint8_t pos) {
        return (uint8_t)((n / fast10Pow(pos)) % 10);
    }

    static inline uint8_t getDigitAt(int64_t n, uint8_t pos) {
        return (uint8_t)((abs(n) / fast10Pow(pos)) % 10);
    }

    static inline uint8_t getDigitAt(uint64_t n, uint8_t pos) {
        return (uint8_t)((n / fast10Pow(pos)) % 10);
    }

    // float — extract digit after scaling
    static inline uint8_t getDigitAt(float n, uint8_t pos, uint8_t decimals) {
        uint32_t scaled = (uint32_t)(abs(n) * fast10Pow(decimals) + 0.5f);
        return (uint8_t)((scaled / fast10Pow(pos)) % 10);
    }


    // ─────────────────────────────────────
    //  Digit Count
    //  countDigits(0)    → 1
    //  countDigits(9)    → 1
    //  countDigits(10)   → 2
    //  countDigits(1234) → 4
    // ─────────────────────────────────────

    static uint8_t countDigits(uint32_t n) {
        if (n == 0) return 1;
        uint8_t count = 0;
        while (n > 0) { n /= 10; count++; }
        return count;
    }

    static uint8_t countDigits(uint64_t n) {
        if (n == 0) return 1;
        uint8_t count = 0;
        while (n > 0) { n /= 10; count++; }
        return count;
    }

    static uint8_t countDigits(int32_t n) {
        return countDigits((uint32_t)(n < 0 ? -n : n));
    }

    static uint8_t countDigits(int64_t n) {
        return countDigits((uint64_t)(n < 0 ? -n : n));
    }

    static uint8_t countDigits(float n, uint8_t decimals) {
        uint32_t scaled = (uint32_t)(abs(n) * fast10Pow(decimals) + 0.5f);
        return countDigits(scaled);
    }


    // ─────────────────────────────────────
    //  Negative Check
    // ─────────────────────────────────────

    static inline bool isNegative(int32_t n) { return n < 0; }
    static inline bool isNegative(int64_t n) { return n < 0; }
    static inline bool isNegative(float n)   { return n < 0.0f; }
    static inline bool isNegative(double n)  { return n < 0.0; }


    // ─────────────────────────────────────
    //  Absolute Value
    //  wraps abs() for all types
    // ─────────────────────────────────────

    static inline uint32_t absVal(int32_t n) {
        return (uint32_t)(n < 0 ? -n : n);
    }

    static inline uint64_t absVal(int64_t n) {
        return (uint64_t)(n < 0 ? -n : n);
    }

    static inline float absVal(float n) {
        return n < 0.0f ? -n : n;
    }


    // ─────────────────────────────────────
    //  Float Scaling
    //  scale float to integer for display
    //  scaleFloat(3.14, 2) → 314
    //  scaleFloat(3.14, 3) → 3140
    // ─────────────────────────────────────

    static inline uint32_t scaleFloat(float n, uint8_t decimals) {
        return (uint32_t)(absVal(n) * fast10Pow(decimals) + 0.5f);
    }

    static inline uint64_t scaleDouble(double n, uint8_t decimals) {
        return (uint64_t)(absVal((float)n) * fast10Pow(decimals) + 0.5);
    }


    // ─────────────────────────────────────
    //  Clamp
    //  keeps value within min/max range
    // ─────────────────────────────────────

    static inline uint8_t clamp(uint8_t val, uint8_t minVal, uint8_t maxVal) {
        if (val < minVal) return minVal;
        if (val > maxVal) return maxVal;
        return val;
    }

    static inline int32_t clamp(int32_t val, int32_t minVal, int32_t maxVal) {
        if (val < minVal) return minVal;
        if (val > maxVal) return maxVal;
        return val;
    }


    // ─────────────────────────────────────
    //  Map
    //  maps value from one range to another
    //  useful for brightness scaling
    //  map(5, 0, 10, 0, 255) → 127
    // ─────────────────────────────────────

    static inline int32_t map(int32_t val,
                               int32_t inMin,  int32_t inMax,
                               int32_t outMin, int32_t outMax) {
        return (val - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }
};
