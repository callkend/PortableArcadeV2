/* 
 * File:   setup.h
 * Author: callwyat
 *
 * Created on April 22, 2019, 8:48 PM
 */

#ifndef Colors_Defined
#define Colors_Defined

#include <stdint.h>

typedef struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
} Color;

const static Color Black = { .R = 0, .G = 0, .B = 0, .A = 0xFF };
const static Color LowestWhite = { .R = 1, .G = 1, .B = 1, .A = 0xFF };

#endif
