
#include <stdbool.h>
#include <stdint.h>

#include "Color.h"
#include "PixiMatrix.h"
#include "PixiGFX.h"

Color PG_GetPixel(PixiMatrix *matrix, uint16_t x, uint16_t y)
{
    return PM_GetPixel(matrix, x, y);
}

void PG_SetPixel(PixiMatrix *matrix, uint16_t x, uint16_t y, Color c)
{

    if (c.A > 0)
    {
        if (c.A < 0xFF) {
            Color current = PM_GetPixel(matrix, x, y);

            uint8_t inverseA = c.A ^ 0xFF;

            c.R = (c.R / inverseA) + (current.R / c.A);
            c.G = (c.G / inverseA) + (current.G / c.A);
            c.R = (c.B / inverseA) + (current.B / c.A);
        }

        PM_SetPixel(matrix, x, y, c);
    }
}

void PG_Fill(PixiMatrix *matrix, Color c)
{
    int16_t maxPixels = (matrix->Width * matrix->Height) + 1;

    uint8_t *pnt = matrix->PixelArray;
    while (--maxPixels > 0)
    {
        *pnt++ = c.G;
        *pnt++ = c.R;
        *pnt++ = c.B;
    }
}

void PG_DrawHorizontalLine(PixiMatrix *matrix, uint16_t sx, uint16_t ex, uint16_t y, Color c)
{
    for (uint16_t x = sx; x < ex; ++x)
    {
        PM_SetPixel(matrix, x, y, c);
    }
}

void PG_DrawVerticalLine(PixiMatrix *matrix, uint16_t x, uint16_t sy, uint16_t ey, Color c)
{
    for (uint16_t y = sy; y < ey; ++y)
    {
        PM_SetPixel(matrix, x, y, c);
    }
}

void PG_DrawRectangle(PixiMatrix *matrix, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, Color c)
{
    PG_DrawHorizontalLine(matrix, sx, ex, sy, c);
    PG_DrawHorizontalLine(matrix, sx, ex, ey, c);
    PG_DrawVerticalLine(matrix, sx, sy + 1, ey - 1, c);
    PG_DrawVerticalLine(matrix, ex, sy + 1, ey - 1, c);
}

void PG_FillRectangle(PixiMatrix *matrix, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, Color c)
{
    for (uint16_t x = sx; x <= ex; ++x)
    {
        for (uint16_t y = sy; y <= ey; ++y)
        {
            PM_SetPixel(matrix, x, y, c);
        }
    }
}

int _PG_DrawChar(PixiMatrix *matrix, PGfxCharacter ch, int cursorX, int cursorY, Color foreColor, Color backColor, bool addCharSpace)
{

    // Make sure the character has a render
    if (ch.Width > 0)
    {
        // Check the the character will render in visible space
        if (cursorX < matrix->Width && (ch.Width + cursorX) > 0 &&
            (cursorY + 8) >= 0 && cursorY < matrix->Height)
        {
            // The character will render on the screen at least a little

            for (int y = 0; y < 8; ++y)
            {
                uint8_t row = ch.Rows[y];
                uint8_t mask = 0x01 << (ch.Width - 1);

                for (int x = 0; x < ch.Width; ++x)
                {
                    int px = cursorX + x;
                    int py = cursorY + y;

                    if (px >= 0 && px < matrix->Width &&
                        py >= 0 && py < matrix->Height)
                    {
                        PG_SetPixel(matrix, px, py, (row & mask) ? foreColor : backColor);
                    }
                    
                    mask = mask >> 1;
                }
            }
        }

        cursorX += ch.Width;
        if (addCharSpace){
            PG_DrawVerticalLine(matrix, cursorX, cursorY, cursorY + 8, backColor);
            ++cursorX;
        }
    }

    return cursorX;
}

int PG_DrawChar(PixiMatrix *matrix, char c, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font)
{
    // Make sure the character is in the ASCII space
    if (c <= 128)
    {
        PGfxCharacter ch = font->Characters[(uint8_t)c];
        _PG_DrawChar(matrix, ch, cursorX, cursorY, foreColor, backColor, true);
    }

    return cursorX;    
}

int PG_DrawText(PixiMatrix *matrix, char *text, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font)
{
    while (*text)
    {
        char c = *text++;
        if (c <= 128)
        {
            PGfxCharacter ch = font->Characters[(uint8_t)c];
            cursorX = _PG_DrawChar(matrix, ch, cursorX, cursorY, foreColor, backColor, true);
        }
    }

    return cursorX;
}

uint16_t PG_GetTextLength(char *text, const PGfxFont *font)
{
    uint16_t result = 0;
    while (*text)
    {
        uint8_t c = (uint8_t)*text++;
        if (c <= 128)
        {
            result += (font->Characters[c].Width + 1);
        }
    }
    
    return --result;
}

int PG_DrawNumber(PixiMatrix *matrix, int16_t number, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font)
{
    const uint16_t decades[] = { 10000, 1000, 100, 10, 1, 0 };

    PGfxCharacter ch; 
    if (number == 0) {
        ch = font->Characters[(uint8_t)'0'];
        return _PG_DrawChar(matrix, ch, cursorX, cursorY, foreColor, backColor, true);

    } else if (number < 0) {
        ch = font->Characters[(uint8_t)'-'];
        cursorX = _PG_DrawChar(matrix, ch, cursorX, cursorY, foreColor, backColor, true);
    }

    const uint16_t *d = decades;
    // Fast forward to the correct multiple of 10
    while (number < *d) {
        ++d;
    }

    while (*d) {

        uint8_t digit = 0;

        while (number >= *d){
            number -= *d;
            ++digit;
        }

        ch = font->Characters[digit + '0'];
        cursorX = _PG_DrawChar(matrix, ch, cursorX, cursorY, foreColor, backColor, true);

        ++d;
    }

    return cursorX;
}