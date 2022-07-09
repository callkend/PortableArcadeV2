/* 
 * File:   setup.h
 * Author: callwyat
 *
 * Created on April 22, 2019, 8:48 PM
 */

#ifndef Colors_Defined
#define Colors_Defined

struct Color {
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

const static struct Color Black = { .R = 0, .G = 0, .B = 0 };
const static struct Color LowestWhite = { .R = 1, .G = 1, .B = 1 };

#endif
