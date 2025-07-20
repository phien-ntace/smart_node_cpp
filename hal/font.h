#pragma once

#include <stdint.h>

typedef struct font {
    uint8_t width;
    uint8_t height;
    const uint16_t *data;
} font_t;


extern font_t font_7x10;
extern font_t font_11x18;
extern font_t font_16x26;