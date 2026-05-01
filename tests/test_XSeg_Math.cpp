// ─────────────────────────────────────────
//  Unit Tests — XSeg_Math
//  Run on desktop with g++ or any test runner
//  No hardware required
// ─────────────────────────────────────────

#include <cassert>
#include <cstdio>
#include "../src/core/XSeg_Math.h"

void test_fast10Pow() {
    assert(XSeg_Math::fast10Pow(0)  == 1ULL);
    assert(XSeg_Math::fast10Pow(1)  == 10ULL);
    assert(XSeg_Math::fast10Pow(2)  == 100ULL);
    assert(XSeg_Math::fast10Pow(3)  == 1000ULL);
    assert(XSeg_Math::fast10Pow(9)  == 1000000000ULL);
    assert(XSeg_Math::fast10Pow(18) == 1000000000000000000ULL);
    assert(XSeg_Math::fast10Pow(19) == 0ULL);   // out of range → 0
    printf("  fast10Pow         PASS\n");
}

void test_getDigitAt() {
    // ones
    assert(XSeg_Math::getDigitAt((int32_t)1234,  0) == 4);
    // tens
    assert(XSeg_Math::getDigitAt((int32_t)1234,  1) == 3);
    // hundreds
    assert(XSeg_Math::getDigitAt((int32_t)1234,  2) == 2);
    // thousands
    assert(XSeg_Math::getDigitAt((int32_t)1234,  3) == 1);
    // negative — should strip sign
    assert(XSeg_Math::getDigitAt((int32_t)-1234, 0) == 4);
    assert(XSeg_Math::getDigitAt((int32_t)-1234, 3) == 1);
    // zero
    assert(XSeg_Math::getDigitAt((int32_t)0,     0) == 0);
    // uint32
    assert(XSeg_Math::getDigitAt((uint32_t)9999, 3) == 9);
    // int64
    assert(XSeg_Math::getDigitAt((int64_t)123456789LL, 0) == 9);
    assert(XSeg_Math::getDigitAt((int64_t)123456789LL, 8) == 1);
    printf("  getDigitAt        PASS\n");
}

void test_countDigits() {
    assert(XSeg_Math::countDigits((uint32_t)0)          == 1);
    assert(XSeg_Math::countDigits((uint32_t)9)          == 1);
    assert(XSeg_Math::countDigits((uint32_t)10)         == 2);
    assert(XSeg_Math::countDigits((uint32_t)99)         == 2);
    assert(XSeg_Math::countDigits((uint32_t)100)        == 3);
    assert(XSeg_Math::countDigits((uint32_t)1234)       == 4);
    assert(XSeg_Math::countDigits((uint32_t)99999999)   == 8);
    assert(XSeg_Math::countDigits((int32_t)-1234)       == 4);
    assert(XSeg_Math::countDigits((int32_t)-9)          == 1);
    printf("  countDigits       PASS\n");
}

void test_isNegative() {
    assert(XSeg_Math::isNegative((int32_t)-1)    == true);
    assert(XSeg_Math::isNegative((int32_t)0)     == false);
    assert(XSeg_Math::isNegative((int32_t)1)     == false);
    assert(XSeg_Math::isNegative(-1.5f)          == true);
    assert(XSeg_Math::isNegative(0.0f)           == false);
    assert(XSeg_Math::isNegative(1.5f)           == false);
    printf("  isNegative        PASS\n");
}

void test_absVal() {
    assert(XSeg_Math::absVal((int32_t)-1234) == 1234u);
    assert(XSeg_Math::absVal((int32_t)1234)  == 1234u);
    assert(XSeg_Math::absVal((int32_t)0)     == 0u);
    assert(XSeg_Math::absVal(-3.14f)         == 3.14f);
    printf("  absVal            PASS\n");
}

void test_scaleFloat() {
    assert(XSeg_Math::scaleFloat(3.14f,  2) == 314u);
    assert(XSeg_Math::scaleFloat(3.14f,  3) == 3140u);
    assert(XSeg_Math::scaleFloat(0.5f,   1) == 5u);
    assert(XSeg_Math::scaleFloat(99.99f, 2) == 9999u);
    assert(XSeg_Math::scaleFloat(-3.14f, 2) == 314u);  // abs applied
    printf("  scaleFloat        PASS\n");
}

void test_clamp() {
    assert(XSeg_Math::clamp((uint8_t)5,  0,  7)  == 5);
    assert(XSeg_Math::clamp((uint8_t)0,  0,  7)  == 0);
    assert(XSeg_Math::clamp((uint8_t)7,  0,  7)  == 7);
    assert(XSeg_Math::clamp((uint8_t)10, 0,  7)  == 7);   // clamp high
    assert(XSeg_Math::clamp((uint8_t)0,  3,  7)  == 3);   // clamp low
    printf("  clamp             PASS\n");
}

void test_map() {
    assert(XSeg_Math::map(5,   0, 10,  0, 100) == 50);
    assert(XSeg_Math::map(0,   0, 10,  0, 255) == 0);
    assert(XSeg_Math::map(10,  0, 10,  0, 255) == 255);
    assert(XSeg_Math::map(5,   0, 10,  0, 15)  == 7);
    printf("  map               PASS\n");
}

int main() {
    printf("\n=== XSeg_Math Tests ===\n");
    test_fast10Pow();
    test_getDigitAt();
    test_countDigits();
    test_isNegative();
    test_absVal();
    test_scaleFloat();
    test_clamp();
    test_map();
    printf("=== All Math Tests PASSED ===\n\n");
    return 0;
}
