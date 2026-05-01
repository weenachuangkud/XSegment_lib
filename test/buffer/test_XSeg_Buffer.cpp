#include <unity.h>
#include "../src/core/XSeg_Buffer.h"

void setUp(void) {}
void tearDown(void) {}

void test_constructor_default(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_clear(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    buf.clearDirty();
    buf.clear();
    TEST_ASSERT_TRUE(buf.isDirty());
    TEST_ASSERT_EQUAL(0x0000, buf.getDigit(0));
}

void test_writeInt_positive_right_align(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_writeInt_negative(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(-123, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    TEST_ASSERT_EQUAL(0x40, buf.getDigit(0)); // minus sign
}

void test_writeInt_left_align(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(12, XSEG_ALIGN_LEFT, XSEG_PAD_SPACE);
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(0));
}

void test_writeInt_leading_zeros(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(42, XSEG_ALIGN_RIGHT, XSEG_PAD_ZERO);
    TEST_ASSERT_EQUAL(0x3F, buf.getDigit(0)); // leading zero
}

void test_writeFloat_basic(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeFloat(3.14f, 2, XSEG_ALIGN_RIGHT);
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_writeString_basic(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeString("ABC", XSEG_ALIGN_LEFT);
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_writeRaw_single(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeRaw(2, 0xFF);
    TEST_ASSERT_EQUAL(0xFF, buf.getDigit(2));
}

void test_blinkTick_off(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    bool result = buf.blinkTick(XSEG_BLINK_OFF);
    TEST_ASSERT_TRUE(result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_constructor_default);
    RUN_TEST(test_clear);
    RUN_TEST(test_writeInt_positive_right_align);
    RUN_TEST(test_writeInt_negative);
    RUN_TEST(test_writeInt_left_align);
    RUN_TEST(test_writeInt_leading_zeros);
    RUN_TEST(test_writeFloat_basic);
    RUN_TEST(test_writeString_basic);
    RUN_TEST(test_writeRaw_single);
    RUN_TEST(test_blinkTick_off);
    return UNITY_END();
}