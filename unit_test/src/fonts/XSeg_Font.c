#include "XSeg_Font.h"

// 7-segment basic font mapping
uint16_t XSeg_Font7_Get(char c) {
    // 7-segment bit positions: A=0, B=1, C=2, D=3, E=4, F=5, G=6, DP=7
    // Segments: A=top, B=top-right, C=bottom-right, D=bottom, E=bottom-left, F=top-left, G=middle
    
    switch (c) {
        case '0': return 0x3F; // ABCDEF
        case '1': return 0x06;  // BC
        case '2': return 0x5B;  // ABDEG
        case '3': return 0x4F;  // ABCD G
        case '4': return 0x66;  // BCFG
        case '5': return 0x6D;  // ACDFG
        case '6': return 0x7D;  // ACDEFG
        case '7': return 0x07;  // ABC
        case '8': return 0x7F;  // ABCDEFG
        case '9': return 0x6F;  // ABCDFG
        
        case 'A': case 'a': return 0x77; // ABCDEF G
        case 'B': case 'b': return 0x7C; // CDEFG
        case 'C': return 0x39; // ADEF
        case 'c': return 0x58; // DEG
        case 'D': case 'd': return 0x5E; // BCDEG
        case 'E': case 'e': return 0x79; // ADEFG
        case 'F': case 'f': return 0x71; // AEFG
        case 'G': case 'g': return 0x6F; // ABCDFG
        case 'H': case 'h': return 0x76; // BCEFG
        case 'I': return 0x06; // BC
        case 'J': case 'j': return 0x1E; // BCDE
        case 'L': case 'l': return 0x38; // DEF
        case 'N': case 'n': return 0x54; // CEG
        case 'O': return 0x3F; // ABCDEF
        case 'P': case 'p': return 0x73; // ABEFG
        case 'Q': case 'q': return 0x67; // ABCFG
        case 'R': case 'r': return 0x50; // EG
        case 'S': case 's': return 0x6D; // ACDFG
        case 'T': case 't': return 0x78; // DEFG
        case 'U': case 'u': return 0x3E; // BCDEF
        case 'V': case 'v': return 0x3E; // BCDEF
        case 'Y': case 'y': return 0x66; // BCDFG
        case 'Z': case 'z': return 0x5B; // ABDEG
        
        case '-': return 0x40; // G
        case '_': return 0x08; // D
        case '=': return 0x48; // DG
        case '.': return 0x80; // DP
        case ' ': return 0x00;
        default:  return 0x00;
    }
}

uint16_t XSeg_Font8_Get(char c) {
    // 8-segment is same as 7-segment with DP
    return XSeg_Font7_Get(c);
}

uint16_t XSeg_Font14_Get(char c) {
    // Simplified 14-segment - use 7-segment approximation
    return XSeg_Font7_Get(c);
}

uint16_t XSeg_Font16_Get(char c) {
    return XSeg_Font14_Get(c);
}