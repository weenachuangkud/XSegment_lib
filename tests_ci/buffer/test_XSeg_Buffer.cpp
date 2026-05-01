#include <unity.h>
#include "../../src/core/XSeg_Buffer.h"

void setUp(void) {}
void tearDown(void) {}

void test_constructor(void) {
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

void test_writeInt_positive(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_writeInt_negative(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(-123, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    TEST_ASSERT_EQUAL(0x40, buf.getDigit(0));
}

void test_writeFloat(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeFloat(3.14f, 2, XSEG_ALIGN_RIGHT);
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_writeString(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeString("ABC", XSEG_ALIGN_LEFT);
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_writeRaw(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeRaw(2, 0xFF);
    TEST_ASSERT_EQUAL(0xFF, buf.getDigit(2));
}

void test_blinkTick(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    TEST_ASSERT_TRUE(buf.blinkTick(XSEG_BLINK_OFF));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_constructor);
    RUN_TEST(test_clear);
    RUN_TEST(test_writeInt_positive);
    RUN_TEST(test_writeInt_negative);
    RUN_TEST(test_writeFloat);
    RUN_TEST(test_writeString);
    RUN_TEST(test_writeRaw);
    RUN_TEST(test_blinkTick);
    return UNITY_END();
}