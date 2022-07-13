
#include <stdint.h>
#include "PixiMatrix.h"

PixiMatrix PM_Init(uint16_t width, uint16_t height, uint8_t *pixelArray, const uint16_t *matrixMap) {
    PixiMatrix result = {
        .Width = width,
        .Height = height,
        .PixelArray = pixelArray,
        .MatrixMap = matrixMap,
    };

    return result;
}

uint8_t* PM_GetLinerLocation(PixiMatrix* matrix, uint16_t x, uint16_t y) {
    
    uint16_t linerLocation = y * matrix->Width + x;
    return &matrix->PixelArray[(matrix->MatrixMap[linerLocation] * 3)];
}

void PM_SetPixel(PixiMatrix* matrix, uint16_t x, uint16_t y, Color color) {
    uint8_t *pnt = PM_GetLinerLocation(matrix, x, y);

    *pnt++ = color.G;
    *pnt++ = color.R;
    *pnt = color.B;
}

Color PM_GetPixel(PixiMatrix* matrix, uint16_t x, uint16_t y) {
    uint8_t *pnt = PM_GetLinerLocation(matrix, x, y);

    Color result = {
        .G = *pnt++,
        .R = *pnt++,
        .B = *pnt++
    };

    return result;
}