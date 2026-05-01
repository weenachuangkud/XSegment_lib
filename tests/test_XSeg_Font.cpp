// ─────────────────────────────────────────
//  Unit Tests — XSeg Fonts
//  Verifies bitmasks are correct
//  against known segment patterns
// ─────────────────────────────────────────

#include <cassert>
#include <cstdio>
#include "../src/core/XSeg_Types.h"
#include "../src/fonts/XSeg_Font.h"

void test_font7_digits() {
    // known correct bitmasks for 7seg digits
    assert(XSeg_GetFont7('0') == 0b00111111);
    assert(XSeg_GetFont7('1') == 0b00000110);
    assert(XSeg_GetFont7('2') == 0b01011011);
    assert(XSeg_GetFont7('3') == 0b01001111);
    assert(XSeg_GetFont7('4') == 0b01100110);
    assert(XSeg_GetFont7('5') == 0b01101101);
    assert(XSeg_GetFont7('6') == 0b01111101);
    assert(XSeg_GetFont7('7') == 0b00000111);
    assert(XSeg_GetFont7('8') == 0b01111111);
    assert(XSeg_GetFont7('9') == 0b01101111);
    printf("  font7 digits      PASS\n");
}

void test_font7_letters() {
    // spot check known letters
    assert(XSeg_GetFont7('A') == XSEG_A);
    assert(XSeg_GetFont7('E') == XSEG_E);
    assert(XSeg_GetFont7('F') == XSEG_F);
    assert(XSeg_GetFont7('H') == XSEG_H);
    assert(XSeg_GetFont7('L') == XSEG_L);
    assert(XSeg_GetFont7('P') == XSEG_P);
    // impossible letters should be blank
    assert(XSeg_GetFont7('K') == XSEG_SPACE);
    assert(XSeg_GetFont7('M') == XSEG_SPACE);
    assert(XSeg_GetFont7('W') == XSEG_SPACE);
    assert(XSeg_GetFont7('X') == XSEG_SPACE);
    printf("  font7 letters     PASS\n");
}

void test_font7_special() {
    assert(XSeg_GetFont7(' ') == XSEG_SPACE);
    assert(XSeg_GetFont7('-') == XSEG_MINUS);
    assert(XSeg_GetFont7('.') == XSEG_DP);
    assert(XSeg_GetFont7('_') == XSEG_UNDERSCORE);
    printf("  font7 special     PASS\n");
}

void test_font8_dp() {
    // digit with DP should have bit 7 set
    uint8_t d3    = XSeg_GetFont8('3', false);
    uint8_t d3_dp = XSeg_GetFont8('3', true);
    assert((d3_dp & 0x80) != 0);           // DP bit set
    assert((d3    & 0x80) == 0);           // DP bit not set
    assert((d3_dp & 0x7F) == (d3 & 0x7F)); // rest same
    printf("  font8 DP          PASS\n");
}

void test_font7_lowercase() {
    // lowercase should map same as uppercase
    assert(XSeg_GetFont7('a') == XSeg_GetFont7('A'));
    assert(XSeg_GetFont7('e') == XSeg_GetFont7('E'));
    assert(XSeg_GetFont7('f') == XSeg_GetFont7('F'));
    printf("  font7 lowercase   PASS\n");
}

void test_font14_full_alphabet() {
    // 14seg should have non-zero for all A-Z
    // including K M W X which 7seg cant do
    assert(XSeg_GetFont14('K') != 0);
    assert(XSeg_GetFont14('M') != 0);
    assert(XSeg_GetFont14('W') != 0);
    assert(XSeg_GetFont14('X') != 0);
    printf("  font14 full alpha PASS\n");
}

void test_font16_full_alphabet() {
    // 16seg should have non-zero for all A-Z
    assert(XSeg_GetFont16('K') != 0);
    assert(XSeg_GetFont16('M') != 0);
    assert(XSeg_GetFont16('W') != 0);
    assert(XSeg_GetFont16('X') != 0);
    printf("  font16 full alpha PASS\n");
}

void test_segment_defines() {
    // verify segment bit defines are correct
    assert(XSEG_SEG_A  == 0x01);
    assert(XSEG_SEG_B  == 0x02);
    assert(XSEG_SEG_C  == 0x04);
    assert(XSEG_SEG_D  == 0x08);
    assert(XSEG_SEG_E  == 0x10);
    assert(XSEG_SEG_F  == 0x20);
    assert(XSEG_SEG_G  == 0x40);
    assert(XSEG_SEG_DP == 0x80);
    printf("  segment defines   PASS\n");
}

void test_digit_defines() {
    // verify digit defines match font tables
    assert(XSEG_0 == XSeg_GetFont7('0'));
    assert(XSEG_1 == XSeg_GetFont7('1'));
    assert(XSEG_2 == XSeg_GetFont7('2'));
    assert(XSEG_3 == XSeg_GetFont7('3'));
    assert(XSEG_4 == XSeg_GetFont7('4'));
    assert(XSEG_5 == XSeg_GetFont7('5'));
    assert(XSEG_6 == XSeg_GetFont7('6'));
    assert(XSEG_7 == XSeg_GetFont7('7'));
    assert(XSEG_8 == XSeg_GetFont7('8'));
    assert(XSEG_9 == XSeg_GetFont7('9'));
    printf("  digit defines     PASS\n");
}

int main() {
    printf("\n=== XSeg_Font Tests ===\n");
    test_font7_digits();
    test_font7_letters();
    test_font7_special();
    test_font8_dp();
    test_font7_lowercase();
    test_font14_full_alphabet();
    test_font16_full_alphabet();
    test_segment_defines();
    test_digit_defines();
    printf("=== All Font Tests PASSED ===\n\n");
    return 0;
}
