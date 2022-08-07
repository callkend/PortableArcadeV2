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
    
#define LCD_0 0x3F
#define LCD_1 0x06
#define LCD_2 0x5B
#define LCD_3 0x4F
#define LCD_4 0x66
#define LCD_5 0x6D
#define LCD_6 0x7D
#define LCD_7 0x07
#define LCD_8 0x7F
#define LCD_9 0x6F
#define LCD_A 0x77
#define LCD_B 0x7C
#define LCD_C 0x39
#define LCD_D 0x5E
#define LCD_E 0x79
#define LCD_F 0x71

#define LCD_G 0x7D      // Same as 6
#define LCD_H 0x74
#define LCD_I 0x30
#define LCD_J 0x1E
#define LCD_K 0x76
#define LCD_L 0x38
#define LCD_M 0x37
#define LCD_N 0x54
#define LCD_O 0x5C
#define LCD_P 0x73
#define LCD_Q 0x67
#define LCD_R 0x50
#define LCD_S 0x6D      // Same as 5
#define LCD_T 0x31
#define LCD_U 0x62
#define LCD_V 0x1C
#define LCD_W 0x3E
#define LCD_X 0x76      // Same as K
#define LCD_Y 0x6E
#define LCD_Z 0x5B      // Same as 2
    
#define LCD_BLANK 0x00
#define LCD_AND 0x7B

    
#ifdef	__cplusplus
}
#endif

#endif	/* LCDTEXT_H */

