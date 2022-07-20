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
#include "Color.h"
#include "PixiMatrix.h"
    
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


    Color PG_GetPixel(PixiMatrix* matrix, uint16_t x, uint16_t y);

    void PG_SetPixel(PixiMatrix* matrix, uint16_t x, uint16_t y, Color c);

    void PG_Fill(PixiMatrix* matrix, Color c);

    void PG_DrawHorizontalLine(PixiMatrix* matrix, uint16_t sx, uint16_t ex, uint16_t y, Color c);

    void PG_DrawVerticalLine(PixiMatrix* matrix, uint16_t x, uint16_t sy, uint16_t ey, Color c);

    void PG_DrawRectangle(PixiMatrix* matrix, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, Color c);

    void PG_FillRectangle(PixiMatrix* matrix, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, Color c);

    int PG_DrawChar(PixiMatrix *matrix, char c, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font);

    int PG_DrawText(PixiMatrix *matrix, char *text, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font);

    uint16_t PG_GetTextLength(char *text, const PGfxFont *font);

#ifdef	__cplusplus
}
#endif

#endif	/* PIXIFGFX_H */

