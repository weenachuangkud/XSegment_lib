#include <unity.h>
#include "core/XSeg_Math.h"

void setUp(void) {}

void tearDown(void) {}

void test_fast10Pow(void) {
    TEST_ASSERT_EQUAL(1ULL,        XSeg_Math::fast10Pow(0));
    TEST_ASSERT_EQUAL(10ULL,       XSeg_Math::fast10Pow(1));
    TEST_ASSERT_EQUAL(100ULL,      XSeg_Math::fast10Pow(2));
    TEST_ASSERT_EQUAL(1000ULL,     XSeg_Math::fast10Pow(3));
    TEST_ASSERT_EQUAL(10000ULL,    XSeg_Math::fast10Pow(4));
    TEST_ASSERT_EQUAL(1000000ULL,   XSeg_Math::fast10Pow(6));
    TEST_ASSERT_EQUAL(1000000000ULL, XSeg_Math::fast10Pow(9));
    TEST_ASSERT_EQUAL(0ULL,        XSeg_Math::fast10Pow(20));
}

void test_getDigitAt_int32(void) {
    TEST_ASSERT_EQUAL(4, XSeg_Math::getDigitAt(1234, 0));
    TEST_ASSERT_EQUAL(3, XSeg_Math::getDigitAt(1234, 1));
    TEST_ASSERT_EQUAL(2, XSeg_Math::getDigitAt(1234, 2));
    TEST_ASSERT_EQUAL(1, XSeg_Math::getDigitAt(1234, 3));
    
    TEST_ASSERT_EQUAL(0, XSeg_Math::getDigitAt(1050, 2));
}

void test_getDigitAt_negative(void) {
    TEST_ASSERT_EQUAL(4, XSeg_Math::getDigitAt(-1234, 0));
    TEST_ASSERT_EQUAL(3, XSeg_Math::getDigitAt(-1234, 1));
    TEST_ASSERT_EQUAL(2, XSeg_Math::getDigitAt(-1234, 2));
    TEST_ASSERT_EQUAL(1, XSeg_Math::getDigitAt(-1234, 3));
}

void test_getDigitAt_uint32(void) {
    TEST_ASSERT_EQUAL(9, XSeg_Math::getDigitAt(4294967295UL, 0));
    TEST_ASSERT_EQUAL(2, XSeg_Math::getDigitAt(12345678UL, 5));
}

void test_countDigits(void) {
    TEST_ASSERT_EQUAL(1, XSeg_Math::countDigits(0));
    TEST_ASSERT_EQUAL(1, XSeg_Math::countDigits(9));
    TEST_ASSERT_EQUAL(2, XSeg_Math::countDigits(10));
    TEST_ASSERT_EQUAL(3, XSeg_Math::countDigits(100));
    TEST_ASSERT_EQUAL(4, XSeg_Math::countDigits(1234));
    TEST_ASSERT_EQUAL(10, XSeg_Math::countDigits(1234567890UL));
}

void test_countDigits_negative(void) {
    TEST_ASSERT_EQUAL(1, XSeg_Math::countDigits(-9));
    TEST_ASSERT_EQUAL(2, XSeg_Math::countDigits(-10));
    TEST_ASSERT_EQUAL(4, XSeg_Math::countDigits(-1234));
}

void test_countDigits_float(void) {
    TEST_ASSERT_EQUAL(3, XSeg_Math::countDigits(3.14f, 2));
    TEST_ASSERT_EQUAL(4, XSeg_Math::countDigits(0.001f, 3));
    TEST_ASSERT_EQUAL(2, XSeg_Math::countDigits(9.99f, 2));
}

void test_isNegative(void) {
    TEST_ASSERT_TRUE(XSeg_Math::isNegative((int32_t)-1));
    TEST_ASSERT_TRUE(XSeg_Math::isNegative((int64_t)-999));
    TEST_ASSERT_TRUE(XSeg_Math::isNegative(-1.5f));
    TEST_ASSERT_TRUE(XSeg_Math::isNegative(-2.5));
    
    TEST_ASSERT_FALSE(XSeg_Math::isNegative((int32_t)0));
    TEST_ASSERT_FALSE(XSeg_Math::isNegative((int32_t)1));
    TEST_ASSERT_FALSE(XSeg_Math::isNegative(0.0f));
}

void test_absVal(void) {
    TEST_ASSERT_EQUAL(123, XSeg_Math::absVal((int32_t)-123));
    TEST_ASSERT_EQUAL(0, XSeg_Math::absVal((int32_t)0));
    TEST_ASSERT_EQUAL(999999, XSeg_Math::absVal((int64_t)-999999));
    TEST_ASSERT_EQUAL_FLOAT(3.14f, XSeg_Math::absVal(-3.14f));
}

void test_scaleFloat(void) {
    TEST_ASSERT_EQUAL(314, XSeg_Math::scaleFloat(3.14f, 2));
    TEST_ASSERT_EQUAL(3140, XSeg_Math::scaleFloat(3.14f, 3));
    TEST_ASSERT_EQUAL(1, XSeg_Math::scaleFloat(0.001f, 3));
    TEST_ASSERT_EQUAL(100, XSeg_Math::scaleFloat(0.1f, 1));
}

void test_clamp(void) {
    TEST_ASSERT_EQUAL(5, XSeg_Math::clamp(5, 0, 10));
    TEST_ASSERT_EQUAL(0, XSeg_Math::clamp(-1, 0, 10));
    TEST_ASSERT_EQUAL(10, XSeg_Math::clamp(15, 0, 10));
    
    TEST_ASSERT_EQUAL(50, XSeg_Math::clamp(50, 0, 100));
    TEST_ASSERT_EQUAL(-10, XSeg_Math::clamp(-10, -100, 100));
    TEST_ASSERT_EQUAL(100, XSeg_Math::clamp(200, -100, 100));
}

void test_map(void) {
    TEST_ASSERT_EQUAL(127, XSeg_Math::map(5, 0, 10, 0, 255));
    TEST_ASSERT_EQUAL(0, XSeg_Math::map(0, 0, 10, 0, 255));
    TEST_ASSERT_EQUAL(255, XSeg_Math::map(10, 0, 10, 0, 255));
    TEST_ASSERT_EQUAL(50, XSeg_Math::map(5, 0, 10, 0, 100));
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_fast10Pow);
    RUN_TEST(test_getDigitAt_int32);
    RUN_TEST(test_getDigitAt_negative);
    RUN_TEST(test_getDigitAt_uint32);
    RUN_TEST(test_countDigits);
    RUN_TEST(test_countDigits_negative);
    RUN_TEST(test_countDigits_float);
    RUN_TEST(test_isNegative);
    RUN_TEST(test_absVal);
    RUN_TEST(test_scaleFloat);
    RUN_TEST(test_clamp);
    RUN_TEST(test_map);
    
    return UNITY_END();
}