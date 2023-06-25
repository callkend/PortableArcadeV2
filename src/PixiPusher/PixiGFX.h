/* 
 * File:   PixiFGFX.h
 * Author: callwyat
 *
 * Created on July 10, 2022, 4:08 PM
 */

#ifndef PIXIFGFX_H
#define	PIXIFGFX_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "Color.h"
#include "PixiMatrix.h"
    
    typedef struct PixiGFX {
        PixiMatrix *Matrix;
        uint8_t *Layer1;
        uint8_t Dimness            : 3;
        unsigned WillUseBrightness : 1;
        uint8_t Brightness         : 3;

    } PixiGFX;

    typedef struct PGfxCharacter {
        uint8_t Width;
        uint8_t Rows[8];
    } PGfxCharacter;

#define DEFINE_Blank_PGfxChar(width) {      \
    .Width = width,                         \
    .Rows = { 0, 0, 0, 0, 0, 0, 0, 0 }      \
}
    
#define DEFINE_PGfxChar(width, r0, r1, r2, r3, r4, r5, r6, r7) {  \
    .Width = width,                                               \
    .Rows = { r0, r1, r2, r3, r4, r5, r6 , r7 }                   \
}

    typedef struct PGfxFont {
        PGfxCharacter Characters[128];
    } PGfxFont;


    PixiGFX PG_Init(PixiMatrix *matrix);

    void PG_SetBrightness(PixiGFX *graphics, uint8_t brightness);

    Color PG_GetPixel(PixiGFX *graphics, uint16_t x, uint16_t y);

    void PG_SetPixel(PixiGFX *graphics, uint16_t x, uint16_t y, Color c);

    void PG_Fill(PixiGFX *graphics, Color c);

    void PG_DrawHorizontalLine(PixiGFX *graphics, uint16_t sx, uint16_t ex, uint16_t y, Color c);

    void PG_DrawVerticalLine(PixiGFX *graphics, uint16_t x, uint16_t sy, uint16_t ey, Color c);

    void PG_DrawRectangle(PixiGFX *graphics, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, Color c);

    void PG_FillRectangle(PixiGFX *graphics, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, Color c);

    int PG_DrawChar(PixiGFX *graphics, char c, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font);

    int PG_DrawText(PixiGFX *graphics, char *text, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font);

    int PG_DrawTextC(PixiGFX *graphics, const char *text, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font);

    uint16_t PG_GetTextLength(char *text, const PGfxFont *font);

    uint16_t PG_GetTextLengthC(const char *text, const PGfxFont *font);


    int PG_DrawNumber(PixiGFX *graphics, int16_t number, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font);

#ifdef	__cplusplus
}
#endif

#endif	/* PIXIFGFX_H */

