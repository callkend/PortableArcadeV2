/* 
 * File:   setup.h
 * Author: callwyat
 *
 * Created on April 22, 2019, 8:48 PM
 */

#ifndef Colors_Defined
#define Colors_Defined

#include <stdint.h>

typedef union {
    struct {
        uint8_t R;
        uint8_t G;
        uint8_t B;
        uint8_t A;
    };
    struct {
        uint32_t RGB  : 24;
    };
    struct {
        uint32_t RGBA : 32;
    };
} Color;

const static Color White = { .R = 255, .G = 255, .B = 255, .A = 0xFF };
const static Color Black = { .R = 0, .G = 0, .B = 0, .A = 0xFF };
const static Color Red = { .R = 255, .G = 0, .B = 0, .A = 0xFF };
const static Color Orange = { .R = 255, .G = 64, .B = 0, .A = 0xFF };
const static Color Green = { .R = 0, .G = 255, .B = 0, .A = 0xFF };
const static Color Blue = { .R = 0, .G = 0, .B = 255, .A = 0xFF };
const static Color LowestWhite = { .R = 1, .G = 1, .B = 1, .A = 0xFF };
const static Color Transparent = { .R = 0, .G = 0, .B = 0, .A = 0 };

#endif
