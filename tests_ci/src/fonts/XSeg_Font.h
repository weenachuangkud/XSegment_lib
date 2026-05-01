#pragma once
#include <stdint.h>

uint16_t XSeg_Font7_Get(char c);
uint16_t XSeg_Font8_Get(char c);

#define XSeg_GetFont7(c)  XSeg_Font7_Get(c)
#define XSeg_GetFont8(c)  XSeg_Font8_Get(c)
#define XSeg_GetFont14(c) XSeg_Font7_Get(c)
#define XSeg_GetFont16(c) XSeg_Font7_Get(c)