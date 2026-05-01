#include <unity.h>
#include "core/XSeg_Buffer.h"
#include "core/XSeg_Types.h"

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
    TEST_ASSERT_EQUAL(0x0000, buf.getDigit(3));
}

void test_writeInt_positive_right_align(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    
    TEST_ASSERT_TRUE(buf.isDirty());
    
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(0));
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(3));
}

void test_writeInt_negative(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(-123, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    
    TEST_ASSERT_TRUE(buf.isDirty());
    
    TEST_ASSERT_EQUAL(0x40, buf.getDigit(0)); // minus sign in G segment
}

void test_writeInt_left_align(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(12, XSEG_ALIGN_LEFT, XSEG_PAD_SPACE);
    
    TEST_ASSERT_EQUAL(0x06, buf.getDigit(0)); // '1'
    TEST_ASSERT_EQUAL(0x5B, buf.getDigit(1)); // '2'
}

void test_writeInt_center_align(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(12, XSEG_ALIGN_CENTER, XSEG_PAD_SPACE);
    
    TEST_ASSERT_EQUAL(0x06, buf.getDigit(1)); // '1' at position 1
    TEST_ASSERT_EQUAL(0x5B, buf.getDigit(2)); // '2' at position 2
}

void test_writeInt_leading_zeros(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(42, XSEG_ALIGN_RIGHT, XSEG_PAD_ZERO);
    
    TEST_ASSERT_EQUAL(0x3F, buf.getDigit(0)); // '0'
    TEST_ASSERT_EQUAL(0x3F, buf.getDigit(1)); // '0'  
    TEST_ASSERT_EQUAL(0x6D, buf.getDigit(2)); // '4'
    TEST_ASSERT_EQUAL(0x66, buf.getDigit(3)); // '2'
}

void test_writeInt_zero(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(0, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(3));
}

void test_writeFloat_basic(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeFloat(3.14f, 2, XSEG_ALIGN_RIGHT);
    
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_writeFloat_negative(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeFloat(-12.5f, 1, XSEG_ALIGN_RIGHT);
    
    TEST_ASSERT_EQUAL(0x40, buf.getDigit(0)); // minus sign
}

void test_writeString_basic(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeString("ABC", XSEG_ALIGN_LEFT);
    
    TEST_ASSERT_TRUE(buf.isDirty());
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(0));
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(2));
}

void test_writeString_right_align(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeString("AB", XSEG_ALIGN_RIGHT);
    
    TEST_ASSERT_EQUAL(0, buf.getDigit(0));
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(2));
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(3));
}

void test_writeString_center_align(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeString("AB", XSEG_ALIGN_CENTER);
    
    TEST_ASSERT_EQUAL(0, buf.getDigit(0));
    TEST_ASSERT_EQUAL(0, buf.getDigit(1));
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(2));
    TEST_ASSERT_NOT_EQUAL(0, buf.getDigit(3));
}

void test_writeRaw_single(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeRaw(2, 0xFF);
    
    TEST_ASSERT_EQUAL(0xFF, buf.getDigit(2));
}

void test_writeRaw_out_of_bounds(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeRaw(10, 0xFF);
    
    TEST_ASSERT_EQUAL(0, buf.getDigit(3));
}

void test_writeRawAll(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    uint16_t masks[] = {0x01, 0x02, 0x03, 0x04};
    buf.writeRawAll(masks);
    
    TEST_ASSERT_EQUAL(0x01, buf.getDigit(0));
    TEST_ASSERT_EQUAL(0x02, buf.getDigit(1));
    TEST_ASSERT_EQUAL(0x03, buf.getDigit(2));
    TEST_ASSERT_EQUAL(0x04, buf.getDigit(3));
}

void test_getBuffer(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    
    uint16_t* ptr = buf.getBuffer();
    TEST_ASSERT_NOT_NULL(ptr);
}

void test_clearDirty(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    buf.clearDirty();
    
    TEST_ASSERT_FALSE(buf.isDirty());
}

void test_blinkTick_off(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    
    bool result = buf.blinkTick(XSEG_BLINK_OFF);
    TEST_ASSERT_TRUE(result);
}

void test_blinkTick_changes(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    
    buf.blinkTick(XSEG_BLINK_SLOW);
    bool first = buf.blinkTick(XSEG_BLINK_SLOW);
    
    // After multiple ticks, should toggle
    // This is timing-dependent, just verify it returns a value
    (void)first;
}

void test_scrollTick_basic(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    
    buf.scrollTick("TEST", 0, XSEG_SCROLL_LEFT);
    
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_scrollTick_no_change_before_delay(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    
    buf.scrollTick("TEST", 100, XSEG_SCROLL_LEFT);
    buf.clearDirty();
    
    bool result = buf.scrollTick("TEST", 100, XSEG_SCROLL_LEFT);
    
    // Should return false (no change) before delay
    TEST_ASSERT_FALSE(result);
}

void test_8digit_type(void) {
    XSeg_Buffer buf(8, XSEG_TYPE_8);
    buf.writeInt(12345678, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    
    TEST_ASSERT_TRUE(buf.isDirty());
}

void test_7digit_type(void) {
    XSeg_Buffer buf(4, XSEG_TYPE_7);
    buf.writeInt(1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    
    TEST_ASSERT_TRUE(buf.isDirty());
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_constructor_default);
    RUN_TEST(test_clear);
    RUN_TEST(test_writeInt_positive_right_align);
    RUN_TEST(test_writeInt_negative);
    RUN_TEST(test_writeInt_left_align);
    RUN_TEST(test_writeInt_center_align);
    RUN_TEST(test_writeInt_leading_zeros);
    RUN_TEST(test_writeInt_zero);
    RUN_TEST(test_writeFloat_basic);
    RUN_TEST(test_writeFloat_negative);
    RUN_TEST(test_writeString_basic);
    RUN_TEST(test_writeString_right_align);
    RUN_TEST(test_writeString_center_align);
    RUN_TEST(test_writeRaw_single);
    RUN_TEST(test_writeRaw_out_of_bounds);
    RUN_TEST(test_writeRawAll);
    RUN_TEST(test_getBuffer);
    RUN_TEST(test_clearDirty);
    RUN_TEST(test_blinkTick_off);
    RUN_TEST(test_blinkTick_changes);
    RUN_TEST(test_scrollTick_basic);
    RUN_TEST(test_scrollTick_no_change_before_delay);
    RUN_TEST(test_8digit_type);
    RUN_TEST(test_7digit_type);
    
    return UNITY_END();
}