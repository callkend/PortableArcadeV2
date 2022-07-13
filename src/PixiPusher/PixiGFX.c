
#include "Color.h"
#include "PixiMatrix.h"

Color PG_GetPixel(PixiMatrix *matrix, uint16_t x, uint16_t y)
{
    return PM_GetPixel(matrix, x, y);
}

void PG_SetPixel(PixiMatrix *matrix, uint16_t x, uint16_t y, Color c)
{
    PM_SetPixel(matrix, x, y, c);
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

