
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
    for (uint16_t x = sx; x <= ex; ++x)
    {
        PM_SetPixel(matrix, x, y, c);
    }
}

void PG_DrawVerticalLine(PixiMatrix *matrix, uint16_t x, uint16_t sy, uint16_t ey, Color c)
{
    for (uint16_t y = sy; x <= ey; ++y)
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

void _PF_DrawChar(PixiMatrix *matrix, PGfxCharacter ch, int cursorX, int cursorY, Color foreColor, Color backColor)
{

    // Make sure the character has a render
    if (ch.Width > 0)
    {
        // Check the the character will render in visible space
        if (cursorX < matrix->Width && (ch.Width + cursorX) > 0 &&
            (cursorY + 8) >= 0 && cursorY < matrix->Height)
        {
            // The character will render on the screen at least a little

            for (int x = 0; x < ch.Width; ++x)
            {

                uint8_t row = ch.Rows[x];

                for (int y = 0; y < 8; ++y)
                {

                    int px = cursorX + x;
                    int py = cursorY + y;

                    if (px > 0 && px < matrix->Width &&
                        py > 0 && py < matrix->Height)
                    {

                        PG_SetPixel(matrix, px, py, (row & 0b1000000) ? foreColor : backColor);

                        row = row << 1;
                    }
                }
            }
        }
    }
}

void PF_DrawChar(PixiMatrix *matrix, char c, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font)
{

    // Make sure the character is in the ASCII space
    if (c <= 128)
    {
        PGfxCharacter ch = font->Characters[(uint8_t)c];
        _PF_DrawChar(matrix, ch, cursorX, cursorY, foreColor, backColor);
    }
}

void PF_DrawText(PixiMatrix *matrix, char *text, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font)
{

    while (*text)
    {
        char c = *text++;
        if (c <= 128)
        {
            PGfxCharacter ch = font->Characters[(uint8_t)c];
            _PF_DrawChar(matrix, ch, cursorX, cursorY, foreColor, backColor);

            cursorX = cursorX + ch.Width + 1;
        }
    }
}
