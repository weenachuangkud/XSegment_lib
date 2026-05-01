#pragma once
#include <stdint.h>

#define XSeg_GetFont7(c)  XSeg_Font7_Get(c)
#define XSeg_GetFont8(c)  XSeg_Font8_Get(c)
#define XSeg_GetFont14(c) XSeg_Font14_Get(c)
#define XSeg_GetFont16(c) XSeg_Font16_Get(c)

uint16_t XSeg_Font7_Get(char c);
uint16_t XSeg_Font8_Get(char c);
uint16_t XSeg_Font14_Get(char c);
uint16_t XSeg_Font16_Get(char c);