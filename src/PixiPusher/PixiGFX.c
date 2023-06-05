
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Color.h"
#include "PixiPixel.h"
#include "PixiMatrix.h"
#include "PixiGFX.h"

PixiGFX PG_Init(PixiMatrix *matrix){

    PixiGFX result = {
        .Matrix = matrix,
        .Brightness = 7,
        .Dimness = 0,
        .Layer1 = NULL
    };
     
    result.Layer1 = (uint8_t*)malloc(matrix->Width * matrix->Height * PixelSize);

    // Trap the program, the Brightness layer can't be used
    while (result.Layer1 == NULL);

    // Copy the current display values into the brightness layer
    uint16_t count = (matrix->Width * matrix->Width);
    uint8_t *matrixPnt = matrix->PixelArray;
    uint8_t *bLayerPnt = result.Layer1;
    
    while (--count > 0)
    {
        *bLayerPnt++ = *matrixPnt++;
        *bLayerPnt++ = *matrixPnt++;
        *bLayerPnt++ = *matrixPnt++;
    }
    
    return result;
}

void PG_SetBrightness(PixiGFX *graphics, uint8_t brightness)
{
    brightness &= 0x07;
    uint8_t dimness = (~brightness & 0x07);
    
    // Only do work if needed
    if (graphics->Brightness != brightness){
        
        uint16_t count = (graphics->Matrix->Width * graphics->Matrix->Height);
        
        uint8_t *matrixPnt = graphics->Matrix->PixelArray;
        uint8_t *bLayerPnt = graphics->Layer1;
        
        // Re-render the image at the new brightness level
        while (--count > 0)
        {
            *matrixPnt++ = *bLayerPnt++ >> dimness;
            *matrixPnt++ = *bLayerPnt++ >> dimness;
            *matrixPnt++ = *bLayerPnt++ >> dimness;
        }
        
        graphics->Brightness = brightness;
        graphics->Dimness = dimness;
    }
}

Color PG_GetPixel(PixiGFX *graphics, uint16_t x, uint16_t y)
{
    uint8_t *pnt = &graphics->Layer1[PM_GetLinerOffset(graphics->Matrix, x, y)];

    Color result = {
        .A = 0xFF,
        .G = *pnt++,
        .R = *pnt++,
        .B = *pnt,
    };
    return result;
}

void PG_SetPixel(PixiGFX *graphics, uint16_t x, uint16_t y, Color c)
{
    if (c.A > 0)
    {
        // Handle Transparency
        if (c.A < 0xFF) {
            Color current = PG_GetPixel(graphics, x, y);

            uint8_t inverseA = c.A ^ 0xFF;

            c.R = (c.R / inverseA) + (current.R / c.A);
            c.G = (c.G / inverseA) + (current.G / c.A);
            c.R = (c.B / inverseA) + (current.B / c.A);
        }

        uint16_t offset = PM_GetLinerOffset(graphics->Matrix, x, y);
        
        // Set the raw value
        uint8_t *pntB = &graphics->Layer1[offset];
        uint8_t *pntM = &graphics->Matrix->PixelArray[offset];
        
        *pntB++ = c.G;
        *pntM++ = c.G >> graphics->Dimness;
        
        *pntB++ = c.R;
        *pntM++ = c.R >> graphics->Dimness;
        
        *pntB   = c.B;
        *pntM   = c.B >> graphics->Dimness;
    }
}

void PG_Fill(PixiGFX *graphics, Color c)
{
    int16_t maxPixels = (graphics->Matrix->Width * graphics->Matrix->Height) + 1;
    uint8_t *pntB = graphics->Layer1;
    uint8_t *pntM = graphics->Matrix->PixelArray;
    while (--maxPixels > 0)
    {
        *pntB++ = c.G;
        *pntM++ = c.G;
        
        *pntB++ = c.R;
        *pntM++ = c.R;
        
        *pntB++ = c.B;
        *pntM++ = c.B;
    }
}

void PG_DrawHorizontalLine(PixiGFX *graphics, uint16_t sx, uint16_t ex, uint16_t y, Color c)
{
    for (uint16_t x = sx; x < ex; ++x)
    {
        PG_SetPixel(graphics, x, y, c);
    }
}

void PG_DrawVerticalLine(PixiGFX *graphics, uint16_t x, uint16_t sy, uint16_t ey, Color c)
{
    for (uint16_t y = sy; y < ey; ++y)
    {
        PG_SetPixel(graphics, x, y, c);
    }
}

void PG_DrawRectangle(PixiGFX *graphics, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, Color c)
{
    PG_DrawHorizontalLine(graphics, sx, ex, sy, c);
    PG_DrawHorizontalLine(graphics, sx, ex, ey, c);
    PG_DrawVerticalLine(graphics, sx, sy + 1, ey - 1, c);
    PG_DrawVerticalLine(graphics, ex, sy + 1, ey - 1, c);
}

void PG_FillRectangle(PixiGFX *graphics, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, Color c)
{
    for (uint16_t x = sx; x <= ex; ++x)
    {
        for (uint16_t y = sy; y <= ey; ++y)
        {
            PG_SetPixel(graphics, x, y, c);
        }
    }
}

int _PG_DrawChar(PixiGFX *graphics, PGfxCharacter ch, int cursorX, int cursorY, Color foreColor, Color backColor, bool addCharSpace)
{

    // Make sure the character has a render
    if (ch.Width > 0)
    {
        // Check the the character will render in visible space
        if (cursorX < graphics->Matrix->Width && (ch.Width + cursorX) > 0 &&
            (cursorY + 8) >= 0 && cursorY < graphics->Matrix->Height)
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

                    if (px >= 0 && px < graphics->Matrix->Width &&
                        py >= 0 && py < graphics->Matrix->Height)
                    {
                        PG_SetPixel(graphics, px, py, (row & mask) ? foreColor : backColor);
                    }
                    
                    mask = mask >> 1;
                }
            }
        }

        cursorX += ch.Width;
        if (addCharSpace){
            PG_DrawVerticalLine(graphics, cursorX, cursorY, cursorY + 8, backColor);
            ++cursorX;
        }
    }

    return cursorX;
}

int PG_DrawChar(PixiGFX *graphics, char c, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font)
{
    // Make sure the character is in the ASCII space
    if (c <= 128)
    {
        PGfxCharacter ch = font->Characters[(uint8_t)c];
        _PG_DrawChar(graphics, ch, cursorX, cursorY, foreColor, backColor, true);
    }

    return cursorX;    
}

int PG_DrawText(PixiGFX *graphics, char *text, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font)
{
    while (*text)
    {
        char c = *text++;
        if (c <= 128)
        {
            PGfxCharacter ch = font->Characters[(uint8_t)c];
            cursorX = _PG_DrawChar(graphics, ch, cursorX, cursorY, foreColor, backColor, true);
        }
    }

    return cursorX;
}

int PG_DrawTextC(PixiGFX *graphics, const char *text, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font) {

    char *c;
    strcpy(c, text);
    return PG_DrawText(graphics, c, cursorX, cursorY, foreColor, backColor, font);
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

int PG_DrawNumber(PixiGFX *graphics, int16_t number, int cursorX, int cursorY, Color foreColor, Color backColor, const PGfxFont *font)
{
    const uint16_t decades[] = { 10000, 1000, 100, 10, 1, 0 };

    PGfxCharacter ch; 
    if (number == 0) {
        ch = font->Characters[(uint8_t)'0'];
        return _PG_DrawChar(graphics, ch, cursorX, cursorY, foreColor, backColor, true);

    } else if (number < 0) {
        ch = font->Characters[(uint8_t)'-'];
        cursorX = _PG_DrawChar(graphics, ch, cursorX, cursorY, foreColor, backColor, true);
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
        cursorX = _PG_DrawChar(graphics, ch, cursorX, cursorY, foreColor, backColor, true);

        ++d;
    }

    return cursorX;
}