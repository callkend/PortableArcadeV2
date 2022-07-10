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


Color PG_GetPixel(PixiMatrix matrix, uint16_t x, uint16_t y);

void PG_SetPixel(PixiMatrix matrix, uint16_t x, uint16_t y, Color c);

void PG_Fill(PixiMatrix matrix, Color c);

void PG_DrawHorizontalLine(PixiMatrix matrix, uint16_t sx, uint16_t ex, uint16_t y, Color c);

void PG_DrawVerticalLine(PixiMatrix matrix, uint16_t x, uint16_t sy, uint16_t ey, Color c);void PG_DrawRectangle(PixiMatrix matrix, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, Color c);

void PG_DrawRectangle(PixiMatrix matrix, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, Color c);

void PG_FillRectangle(PixiMatrix matrix, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, Color c);




#ifdef	__cplusplus
}
#endif

#endif	/* PIXIFGFX_H */

