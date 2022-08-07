/* 
 * File:   LCDText.h
 * Author: callwyat
 *
 * Created on August 6, 2022, 3:42 PM
 */
#include <stdint.h>

#ifndef LCDTEXT_H
#define	LCDTEXT_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define LCD_0 0x3F00
#define LCD_1 0x0600
#define LCD_2 0x5B00
#define LCD_3 0x4F00
#define LCD_4 0x6600
#define LCD_5 0x6D00
#define LCD_6 0x7D00
#define LCD_7 0x0700
#define LCD_8 0x7F00
#define LCD_9 0x6F00
#define LCD_A 0x7700
#define LCD_B 0x7C00
#define LCD_C 0x3900
#define LCD_D 0x5E00
#define LCD_E 0x7900
#define LCD_F 0x7100

#define LCD_G 0x7D00      // Same as 6
#define LCD_H 0x7400
#define LCD_I 0x3000
#define LCD_J 0x1E00
#define LCD_K 0x7600
#define LCD_L 0x3800
#define LCD_M 0x3700
#define LCD_N 0x5400
#define LCD_O 0x5C00
#define LCD_P 0x7300
#define LCD_Q 0x6700
#define LCD_R 0x5000
#define LCD_S 0x6D00      // Same as 5
#define LCD_T 0x3100
#define LCD_U 0x6200
#define LCD_V 0x1C00
#define LCD_W 0x3E00
#define LCD_X 0x7600      // Same as K
#define LCD_Y 0x6E00
#define LCD_Z 0x5B00      // Same as 2
    
#define LCD_BLANK 0x0000
#define LCD_AND 0x7B00

    
#ifdef	__cplusplus
}
#endif

#endif	/* LCDTEXT_H */

