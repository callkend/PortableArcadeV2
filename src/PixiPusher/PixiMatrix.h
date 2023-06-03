/*
 * File:   PixiMatrix.h
 * Author: callwyat
 *
 * Created on July 10, 2022, 2:00 PM
 */

#ifndef PIXIMATRIX_H
#define PIXIMATRIX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "color.h"

    typedef struct PixiMatrix {
        uint16_t Width;
        uint16_t Height;
        uint8_t *PixelArray;
        const uint16_t *MatrixMap;
    } PixiMatrix;

    PixiMatrix PM_Init(uint16_t width, uint16_t height, uint8_t *pixelArray, const uint16_t *matrixMap);

    uint16_t PM_GetLinerOffset(PixiMatrix* matrix, uint16_t x, uint16_t y);

    void PM_SetPixel(PixiMatrix* matrix, uint16_t x, uint16_t y, Color color);

    Color PM_GetPixel(PixiMatrix* matrix, uint16_t x, uint16_t y);

#ifdef __cplusplus
}
#endif

#endif /* PIXIMATRIX_H */
