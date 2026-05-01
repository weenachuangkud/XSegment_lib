#include "XSeg_Font.h"

uint16_t XSeg_Font7_Get(char c) {
    switch (c) {
        case '0': return 0x3F; case '1': return 0x06; case '2': return 0x5B;
        case '3': return 0x4F; case '4': return 0x66; case '5': return 0x6D;
        case '6': return 0x7D; case '7': return 0x07; case '8': return 0x7F;
        case '9': return 0x6F; case 'A': case 'a': return 0x77;
        case 'B': case 'b': return 0x7C; case 'C': return 0x39;
        case 'D': case 'd': return 0x5E; case 'E': case 'e': return 0x79;
        case 'F': case 'f': return 0x71; case 'G': case 'g': return 0x6F;
        case 'H': case 'h': return 0x76; case 'I': return 0x06;
        case 'J': case 'j': return 0x1E; case 'L': case 'l': return 0x38;
        case 'N': case 'n': return 0x54; case 'O': return 0x3F;
        case 'P': case 'p': return 0x73; case 'Q': case 'q': return 0x67;
        case 'R': case 'r': return 0x50; case 'S': case 's': return 0x6D;
        case 'T': case 't': return 0x78; case 'U': case 'u': return 0x3E;
        case 'Y': case 'y': return 0x66; case 'Z': case 'z': return 0x5B;
        case '-': return 0x40; case '_': return 0x08; case '=': return 0x48;
        case '.': return 0x80; case ' ': return 0x00;
        default: return 0x00;
    }
}

uint16_t XSeg_Font8_Get(char c) { return XSeg_Font7_Get(c); }