#include <unity.h>

// Include core library headers directly (avoid Arduino deps)
#include "../../src/core/XSeg_Types.h"
#include "../../src/core/XSeg_Math.h"
#include "../../src/core/XSeg_Buffer.h"
#include "../../src/fonts/XSeg_Font.h"

void setUp(void) {}
void tearDown(void) {}

void test_library_load(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_writeInt(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_math_fast10pow(void) {
    TEST_ASSERT_EQUAL(1ULL,  XSeg_Math::fast10Pow(0));
    TEST_ASSERT_EQUAL(10ULL, XSeg_Math::fast10Pow(1));
    TEST_ASSERT_EQUAL(100ULL, XSeg_Math::fast10Pow(2));
}

void test_math_getDigitAt(void) {
    TEST_ASSERT_EQUAL(4, XSeg_Math::getDigitAt(1234, 0));
    TEST_ASSERT_EQUAL(3, XSeg_Math::getDigitAt(1234, 1));
    TEST_ASSERT_EQUAL(2, XSeg_Math::getDigitAt(1234, 2));
}

void test_math_countDigits(void) {
    TEST_ASSERT_EQUAL(1, XSeg_Math::countDigits(0));
    TEST_ASSERT_EQUAL(2, XSeg_Math::countDigits(10));
    TEST_ASSERT_EQUAL(4, XSeg_Math::countDigits(1234));
}

void test_math_isNegative(void) {
    TEST_ASSERT_TRUE(XSeg_Math::isNegative(-1));
    TEST_ASSERT_FALSE(XSeg_Math::isNegative(0));
    TEST_ASSERT_FALSE(XSeg_Math::isNegative(1));
}

void test_buffer_clear(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    buf.clearDirty();
    buf.clear();
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_buffer_writeInt_negative(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(-123, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    TEST_ASSERT_EQUAL(0x40, buf.getDigit(0));
}

void test_buffer_writeFloat(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeFloat(3.14f, 2, XSEG_ALIGN_RIGHT);
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_buffer_writeString(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeString("ABC", XSEG_ALIGN_LEFT);
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_buffer_writeRaw(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeRaw(2, 0xFF);
    TEST_ASSERT_EQUAL(0xFF, buf.getDigit(2));
}

void test_buffer_blinkTick(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    TEST_ASSERT_TRUE(buf.blinkTick(XSEG_BLINK_OFF));
}

void test_buffer_leadingZeros(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(42, XSEG_ALIGN_RIGHT, XSEG_PAD_ZERO);
    TEST_ASSERT_EQUAL(0x3F, buf.getDigit(0));
}

void test_math_clamp(void) {
    TEST_ASSERT_EQUAL(5, XSeg_Math::clamp(5, 0, 10));
    TEST_ASSERT_EQUAL(0, XSeg_Math::clamp(-1, 0, 10));
    TEST_ASSERT_EQUAL(10, XSeg_Math::clamp(15, 0, 10));
}

void test_math_map(void) {
    TEST_ASSERT_EQUAL(127, XSeg_Math::map(5, 0, 10, 0, 255));
    TEST_ASSERT_EQUAL(0, XSeg_Math::map(0, 0, 10, 0, 255));
    TEST_ASSERT_EQUAL(255, XSeg_Math::map(10, 0, 10, 0, 255));
}

void test_math_absVal(void) {
    TEST_ASSERT_EQUAL(123, XSeg_Math::absVal(-123));
    TEST_ASSERT_EQUAL(0, XSeg_Math::absVal(0));
}

void test_math_scaleFloat(void) {
    TEST_ASSERT_EQUAL(314, XSeg_Math::scaleFloat(3.14f, 2));
    TEST_ASSERT_EQUAL(3140, XSeg_Math::scaleFloat(3.14f, 3));
}

void test_buffer_types(void) {
    XSeg_Buffer buf7(4, XSEG_TYPE_7);
    buf7.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    TEST_ASSERT_TRUE(buf7.isDirty());

    XSeg_Buffer buf8(4, XSEG_TYPE_8);
    buf8.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    TEST_ASSERT_TRUE(buf8.isDirty());
}

void test_buffer_getBuffer(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    uint16_t* ptr = buf.getBuffer();
    TEST_ASSERT_NOT_NULL(ptr);
}

void test_buffer_clearDirty(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    buf.clearDirty();
    TEST_ASSERT_FALSE(buf.isDirty());
}

void test_buffer_outOfBounds(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeRaw(10, 0xFF);
    TEST_ASSERT_EQUAL(0, buf.getDigit(3));
}

void test_buffer_writeRawAll(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    uint16_t masks[] = {0x01, 0x02, 0x03, 0x04};
    buf.writeRawAll(masks);
    TEST_ASSERT_EQUAL(0x01, buf.getDigit(0));
    TEST_ASSERT_EQUAL(0x02, buf.getDigit(1));
    TEST_ASSERT_EQUAL(0x03, buf.getDigit(2));
    TEST_ASSERT_EQUAL(0x04, buf.getDigit(3));
}

void test_math_getDigitAt_negative(void) {
    TEST_ASSERT_EQUAL(4, XSeg_Math::getDigitAt(-1234, 0));
    TEST_ASSERT_EQUAL(3, XSeg_Math::getDigitAt(-1234, 1));
}

void test_math_countDigits_negative(void) {
    TEST_ASSERT_EQUAL(1, XSeg_Math::countDigits(-9));
    TEST_ASSERT_EQUAL(2, XSeg_Math::countDigits(-10));
    TEST_ASSERT_EQUAL(4, XSeg_Math::countDigits(-1234));
}

void test_math_countDigits_float(void) {
    TEST_ASSERT_EQUAL(3, XSeg_Math::countDigits(3.14f, 2));
    TEST_ASSERT_EQUAL(4, XSeg_Math::countDigits(0.001f, 3));
}

void test_buffer_stringAlignments(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    
    buf.writeString("AB", XSEG_ALIGN_LEFT);
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(0));
    
    buf.clear();
    buf.writeString("AB", XSEG_ALIGN_RIGHT);
    TEST_ASSERT_EQUAL(0, buf.getDigit(0));
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(2));
}

void setup() {
    UNITY_BEGIN();
    
    RUN_TEST(test_library_load);
    RUN_TEST(test_writeInt);
    
    RUN_TEST(test_math_fast10pow);
    RUN_TEST(test_math_getDigitAt);
    RUN_TEST(test_math_countDigits);
    RUN_TEST(test_math_isNegative);
    RUN_TEST(test_math_clamp);
    RUN_TEST(test_math_map);
    RUN_TEST(test_math_absVal);
    RUN_TEST(test_math_scaleFloat);
    RUN_TEST(test_math_getDigitAt_negative);
    RUN_TEST(test_math_countDigits_negative);
    RUN_TEST(test_math_countDigits_float);
    
    RUN_TEST(test_buffer_clear);
    RUN_TEST(test_buffer_writeInt_negative);
    RUN_TEST(test_buffer_writeFloat);
    RUN_TEST(test_buffer_writeString);
    RUN_TEST(test_buffer_writeRaw);
    RUN_TEST(test_buffer_blinkTick);
    RUN_TEST(test_buffer_leadingZeros);
    RUN_TEST(test_buffer_types);
    RUN_TEST(test_buffer_getBuffer);
    RUN_TEST(test_buffer_clearDirty);
    RUN_TEST(test_buffer_outOfBounds);
    RUN_TEST(test_buffer_writeRawAll);
    RUN_TEST(test_buffer_stringAlignments);
    
    UNITY_END();
}

void loop() {}