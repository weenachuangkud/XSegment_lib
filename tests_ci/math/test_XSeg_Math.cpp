#include <unity.h>
#include "../../src/core/XSeg_Math.h"

void setUp(void) {}
void tearDown(void) {}

void test_fast10Pow(void) {
    TEST_ASSERT_EQUAL(1ULL,  XSeg_Math::fast10Pow(0));
    TEST_ASSERT_EQUAL(10ULL, XSeg_Math::fast10Pow(1));
    TEST_ASSERT_EQUAL(100ULL, XSeg_Math::fast10Pow(2));
    TEST_ASSERT_EQUAL(0ULL, XSeg_Math::fast10Pow(20));
}

void test_getDigitAt(void) {
    TEST_ASSERT_EQUAL(4, XSeg_Math::getDigitAt(1234, 0));
    TEST_ASSERT_EQUAL(3, XSeg_Math::getDigitAt(1234, 1));
    TEST_ASSERT_EQUAL(2, XSeg_Math::getDigitAt(1234, 2));
    TEST_ASSERT_EQUAL(1, XSeg_Math::getDigitAt(1234, 3));
    TEST_ASSERT_EQUAL(4, XSeg_Math::getDigitAt(-1234, 0));
}

void test_countDigits(void) {
    TEST_ASSERT_EQUAL(1, XSeg_Math::countDigits(0));
    TEST_ASSERT_EQUAL(2, XSeg_Math::countDigits(10));
    TEST_ASSERT_EQUAL(4, XSeg_Math::countDigits(1234));
}

void test_isNegative(void) {
    TEST_ASSERT_TRUE(XSeg_Math::isNegative(-1));
    TEST_ASSERT_FALSE(XSeg_Math::isNegative(0));
    TEST_ASSERT_FALSE(XSeg_Math::isNegative(1));
}

void test_absVal(void) {
    TEST_ASSERT_EQUAL(123, XSeg_Math::absVal(-123));
    TEST_ASSERT_EQUAL(0, XSeg_Math::absVal(0));
}

void test_scaleFloat(void) {
    TEST_ASSERT_EQUAL(314, XSeg_Math::scaleFloat(3.14f, 2));
    TEST_ASSERT_EQUAL(3140, XSeg_Math::scaleFloat(3.14f, 3));
}

void test_clamp(void) {
    TEST_ASSERT_EQUAL(5, XSeg_Math::clamp(5, 0, 10));
    TEST_ASSERT_EQUAL(0, XSeg_Math::clamp(-1, 0, 10));
    TEST_ASSERT_EQUAL(10, XSeg_Math::clamp(15, 0, 10));
}

void test_map(void) {
    TEST_ASSERT_EQUAL(127, XSeg_Math::map(5, 0, 10, 0, 255));
    TEST_ASSERT_EQUAL(0, XSeg_Math::map(0, 0, 10, 0, 255));
    TEST_ASSERT_EQUAL(255, XSeg_Math::map(10, 0, 10, 0, 255));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fast10Pow);
    RUN_TEST(test_getDigitAt);
    RUN_TEST(test_countDigits);
    RUN_TEST(test_isNegative);
    RUN_TEST(test_absVal);
    RUN_TEST(test_scaleFloat);
    RUN_TEST(test_clamp);
    RUN_TEST(test_map);
    return UNITY_END();
}