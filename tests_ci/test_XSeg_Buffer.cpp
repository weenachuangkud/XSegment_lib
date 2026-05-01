// ─────────────────────────────────────────
//  Unit Tests — XSeg_Buffer
//  Verifies formatting logic
//  alignment, padding, floats, strings
// ─────────────────────────────────────────

#include <cassert>
#include <cstdio>
#include "../src/core/XSeg_Types.h"
#include "../src/core/XSeg_Buffer.h"
#include "../src/fonts/XSeg_Font.h"

void test_buffer_clear() {
    XSeg_Buffer buf(4, XSEG_TYPE_7);
    buf.clear();
    for (uint8_t i = 0; i < 4; i++) {
        assert(buf.getDigit(i) == XSEG_SPACE);
    }
    printf("  buffer clear      PASS\n");
}

void test_buffer_writeInt_right() {
    XSeg_Buffer buf(4, XSEG_TYPE_7);
    buf.writeInt((int32_t)1234, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);

    // right aligned — digit positions
    assert(buf.getDigit(0) == XSeg_GetFont7('1'));
    assert(buf.getDigit(1) == XSeg_GetFont7('2'));
    assert(buf.getDigit(2) == XSeg_GetFont7('3'));
    assert(buf.getDigit(3) == XSeg_GetFont7('4'));
    printf("  writeInt right    PASS\n");
}

void test_buffer_writeInt_left() {
    XSeg_Buffer buf(4, XSEG_TYPE_7);
    buf.writeInt((int32_t)12, XSEG_ALIGN_LEFT, XSEG_PAD_SPACE);

    // left aligned — 12 at positions 0,1 — rest blank
    assert(buf.getDigit(0) == XSeg_GetFont7('1'));
    assert(buf.getDigit(1) == XSeg_GetFont7('2'));
    assert(buf.getDigit(2) == XSEG_SPACE);
    assert(buf.getDigit(3) == XSEG_SPACE);
    printf("  writeInt left     PASS\n");
}

void test_buffer_writeInt_center() {
    XSeg_Buffer buf(4, XSEG_TYPE_7);
    buf.writeInt((int32_t)12, XSEG_ALIGN_CENTER, XSEG_PAD_SPACE);

    // center — 12 at positions 1,2 — blanks around
    assert(buf.getDigit(0) == XSEG_SPACE);
    assert(buf.getDigit(1) == XSeg_GetFont7('1'));
    assert(buf.getDigit(2) == XSeg_GetFont7('2'));
    assert(buf.getDigit(3) == XSEG_SPACE);
    printf("  writeInt center   PASS\n");
}

void test_buffer_writeInt_negative() {
    XSeg_Buffer buf(4, XSEG_TYPE_7);
    buf.writeInt((int32_t)-123, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);

    // -123 → [-, 1, 2, 3]
    assert(buf.getDigit(0) == XSEG_MINUS);
    assert(buf.getDigit(1) == XSeg_GetFont7('1'));
    assert(buf.getDigit(2) == XSeg_GetFont7('2'));
    assert(buf.getDigit(3) == XSeg_GetFont7('3'));
    printf("  writeInt negative PASS\n");
}

void test_buffer_writeInt_leading_zeros() {
    XSeg_Buffer buf(4, XSEG_TYPE_7);
    buf.writeInt((int32_t)12, XSEG_ALIGN_RIGHT, XSEG_PAD_ZERO);

    // leading zeros — [0, 0, 1, 2]
    assert(buf.getDigit(0) == XSeg_GetFont7('0'));
    assert(buf.getDigit(1) == XSeg_GetFont7('0'));
    assert(buf.getDigit(2) == XSeg_GetFont7('1'));
    assert(buf.getDigit(3) == XSeg_GetFont7('2'));
    printf("  writeInt zeros    PASS\n");
}

void test_buffer_writeFloat() {
    XSeg_Buffer buf(4, XSEG_TYPE_8);
    buf.writeFloat(3.14f, 2, XSEG_ALIGN_RIGHT);

    // 3.14 → [3., 1, 4, blank] or [blank, 3., 1, 4]
    // decimal point should be set on digit with 3
    bool dpFound = false;
    for (uint8_t i = 0; i < 4; i++) {
        if (buf.getDigit(i) & XSEG_SEG_DP) {
            dpFound = true;
        }
    }
    assert(dpFound);
    printf("  writeFloat        PASS\n");
}

void test_buffer_writeString() {
    XSeg_Buffer buf(4, XSEG_TYPE_7);
    buf.writeString("HELP", XSEG_ALIGN_LEFT);

    assert(buf.getDigit(0) == XSeg_GetFont7('H'));
    assert(buf.getDigit(1) == XSeg_GetFont7('E'));
    assert(buf.getDigit(2) == XSeg_GetFont7('L'));
    assert(buf.getDigit(3) == XSeg_GetFont7('P'));
    printf("  writeString       PASS\n");
}

void test_buffer_writeRaw() {
    XSeg_Buffer buf(4, XSEG_TYPE_7);
    buf.writeRaw(0, 0x7F);
    buf.writeRaw(2, 0x01);

    assert(buf.getDigit(0) == 0x7F);
    assert(buf.getDigit(2) == 0x01);
    printf("  writeRaw          PASS\n");
}

void test_buffer_dirty_flag() {
    XSeg_Buffer buf(4, XSEG_TYPE_7);
    buf.clear();
    assert(buf.isDirty() == true);
    buf.clearDirty();
    assert(buf.isDirty() == false);
    buf.writeInt((int32_t)1, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);
    assert(buf.isDirty() == true);
    printf("  dirty flag        PASS\n");
}

void test_buffer_zero() {
    XSeg_Buffer buf(4, XSEG_TYPE_7);
    buf.writeInt((int32_t)0, XSEG_ALIGN_RIGHT, XSEG_PAD_SPACE);

    // zero — rightmost digit should be 0
    assert(buf.getDigit(3) == XSeg_GetFont7('0'));
    printf("  writeInt zero     PASS\n");
}

int main() {
    printf("\n=== XSeg_Buffer Tests ===\n");
    test_buffer_clear();
    test_buffer_writeInt_right();
    test_buffer_writeInt_left();
    test_buffer_writeInt_center();
    test_buffer_writeInt_negative();
    test_buffer_writeInt_leading_zeros();
    test_buffer_writeFloat();
    test_buffer_writeString();
    test_buffer_writeRaw();
    test_buffer_dirty_flag();
    test_buffer_zero();
    printf("=== All Buffer Tests PASSED ===\n\n");
    return 0;
}
