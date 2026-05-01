#include <stdio.h>
#include <string.h>
#include <stdint.h>

// ═══════════════════════════════════════════════════════════════
//  XSeg Visual Simulator - Console-based segment display output
// ═══════════════════════════════════════════════════════════════

static const char* SEGMENT_ASCII[][3] = {
    // Horizontal segments: top, middle, bottom
    {" ┌─ ", " │  ", " └─ "},  // 7-segment layout reference
};

// Convert 7-segment pattern to ASCII art
void printSegment7(uint16_t pattern, int pos) {
    // Segments: A=top, B=top-right, C=bottom-right, D=bottom, E=bottom-left, F=top-left, G=middle
    bool segA = (pattern & (1 << 0));
    bool segB = (pattern & (1 << 1));
    bool segC = (pattern & (1 << 2));
    bool segD = (pattern & (1 << 3));
    bool segE = (pattern & (1 << 4));
    bool segF = (pattern & (1 << 5));
    bool segG = (pattern & (1 << 6));
    bool segDP = (pattern & (1 << 7));
    
    printf("  "); // padding
    printf("%c", segA ? '■' : ' '); // top
    printf("────");
    printf("\n");
    
    printf("%d: ", pos);
    printf("%c", segF ? '│' : ' ');  // top-left
    printf("    ");
    printf("%c", segB ? '│' : ' ');  // top-right
    printf("  ");
    printf("\n");
    
    printf("  "); // padding
    printf("%c", segG ? '■' : ' '); // middle
    printf("────");
    printf("\n");
    
    printf("  "); // padding
    printf("%c", segE ? '│' : ' ');  // bottom-left
    printf("    ");
    printf("%c", segC ? '│' : ' ');  // bottom-right
    printf("  ");
    if (segDP) printf("●"); else printf(" ");
    printf("\n");
    
    printf("  "); // padding
    printf("%c", segD ? '■' : ' '); // bottom
    printf("────");
    printf("\n");
}

// Print compact version
void printSegment7Compact(uint16_t pattern) {
    bool segA = (pattern & (1 << 0));
    bool segB = (pattern & (1 << 1));
    bool segC = (pattern & (1 << 2));
    bool segD = (pattern & (1 << 3));
    bool segE = (pattern & (1 << 4));
    bool segF = (pattern & (1 << 5));
    bool segG = (pattern & (1 << 6));
    bool segDP = (pattern & (1 << 7));
    
    // Top row
    printf(" ");
    printf(segA ? " _   " : "     ");
    printf(" ");
    
    // Middle row
    printf("|");
    printf(segF ? "_" : " ");
    printf(segG ? "_" : " ");
    printf(segB ? "_" : " ");
    printf("|");
    printf(" ");
    
    // Bottom row
    printf("|");
    printf(segE ? "_" : " ");
    printf(segD ? "_" : " ");
    printf(segC ? "_" : " ");
    printf("|");
    if (segDP) printf(".");
    printf(" ");
}

// Print all digits in a buffer
void printDisplayBuffer(uint16_t* buffer, uint8_t numDigits, const char* label) {
    printf("\n═══════════════════════════════════════════\n");
    printf("  %s\n", label);
    printf("═══════════════════════════════════════════\n");
    
    for (int i = 0; i < numDigits; i++) {
        printSegment7Compact(buffer[i]);
    }
    printf("\n");
}

// Show all digit patterns 0-9
void demoDigits() {
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║      7-Segment Display Demo 0-9          ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    
    // Standard 7-segment patterns for 0-9
    uint16_t digits[] = {
        0x3F, // 0: ABCDEF
        0x06, // 1: BC
        0x5B, // 2: ABDEG
        0x4F, // 3: ABCD G
        0x66, // 4: BCFG
        0x6D, // 5: ACDFG
        0x7D, // 6: ACDEFG
        0x07, // 7: ABC
        0x7F, // 8: ABCDEFG
        0x6F, // 9: ABCDFG
    };
    
    printf("\n ");
    for (int i = 0; i < 10; i++) {
        printf("  %d  ", i);
    }
    printf("\n");
    
    printf("┌");
    for (int i = 0; i < 10; i++) {
        printf("────");
    }
    printf("┐\n");
    
    // Top
    printf("│");
    for (int i = 0; i < 10; i++) {
        printf(" %c ", (digits[i] & 0x01) ? '_' : ' ');
    }
    printf("│\n");
    
    // Upper middle
    printf("│");
    for (int i = 0; i < 10; i++) {
        printf("%c%c%c", (digits[i] & 0x20) ? '|' : ' ', 
                             (digits[i] & 0x40) ? '_' : ' ',
                             (digits[i] & 0x02) ? '|' : ' ');
    }
    printf("│\n");
    
    // Lower middle
    printf("│");
    for (int i = 0; i < 10; i++) {
        printf("%c%c%c", (digits[i] & 0x10) ? '|' : ' ',
                             (digits[i] & 0x08) ? '_' : ' ',
                             (digits[i] & 0x04) ? '|' : ' ');
    }
    printf("│\n");
    
    printf("└");
    for (int i = 0; i < 10; i++) {
        printf("────");
    }
    printf("┘\n");
}

// Show letters A-Z
void demoLetters() {
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║      7-Segment Display Demo A-Z          ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    
    // Letters that can be displayed on 7-segment
    char letters[] = "ABCDEFGHIJLNOPQRSTUVYZ";
    int count = sizeof(letters) - 1;
    
    for (int row = 0; row < (count + 7) / 8; row++) {
        printf("\n ");
        for (int col = 0; col < 8; col++) {
            int idx = row * 8 + col;
            if (idx < count) {
                printf("  %c  ", letters[idx]);
            } else {
                printf("     ");
            }
        }
        printf("\n");
        
        printf("┌");
        for (int col = 0; col < 8; col++) {
            printf("────");
        }
        printf("┐\n");
    }
}

// Interactive test menu
void runInteractiveTest() {
    demoDigits();
    demoLetters();
    
    printf("\n═══════════════════════════════════════════\n");
    printf("  Interactive Test Complete!\n");
    printf("═══════════════════════════════════════════\n");
    printf("\nTo run with PlatformIO:\n");
    printf("  cd test && pio test -e native\n");
    printf("\nTo run visual simulation:\n");
    printf("  cd test && ./build/native/test_visual\n");
}

#ifndef UNIT_TEST
int main(void) {
    runInteractiveTest();
    return 0;
}
#endif