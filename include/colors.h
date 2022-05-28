#ifndef COLORS_H
#define COLORS_H

enum {
    COLOR_RED,
    COLOR_BRIGHT_ORANGE,
    COLOR_DIM_ORANGE,
    COLOR_YELLOW,
    COLOR_DIM_GREEN,
    COLOR_BRIGHT_GREEN,
    COLOR_BLUE,
    COLOR_TOTAL,
};

static SDL_Color COLORS[] = {
    { .r = 0xf2, .g = 0x3e, .b = 0x41, .a = 0xff, },    // Red
    { .r = 0xf9, .g = 0x8e, .b = 0x0c, .a = 0xff, },    // Bright Orange
    { .r = 0xf4, .g = 0xaa, .b = 0x69, .a = 0xff, },    // Dim Orange
    { .r = 0xf4, .g = 0xd8, .b = 0x5d, .a = 0xff, },    // Yellow
    { .r = 0xa5, .g = 0xb8, .b = 0x5e, .a = 0xff, },    // Dim Green
    { .r = 0x5e, .g = 0xaa, .b = 0x6c, .a = 0xff, },    // Bright Green
    { .r = 0x09, .g = 0x65, .b = 0x96, .a = 0xff, },    // Blue
};

#endif
