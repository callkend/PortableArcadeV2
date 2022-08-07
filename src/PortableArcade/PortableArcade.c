
#include <stdbool.h>
#include <stdint.h>

#include "LCDText.h"

__eds__ uint8_t __attribute__((noload, address(0x008000))) EXTNIN1 __attribute__((space(eds)));
__eds__ uint8_t __attribute__((noload, address(0x008002))) EXTNIN2 __attribute__((space(eds)));

__eds__ uint8_t __attribute__((noload, address(0x008004))) EXT_RES2 __attribute__((space(eds)));

__eds__ uint8_t __attribute__((noload, address(0x008006))) EXTNOUT1 __attribute__((space(eds)));

__eds__ uint8_t __attribute__((noload, address(0x008008))) EXT_RES4 __attribute__((space(eds)));    
__eds__ uint8_t __attribute__((noload, address(0x00800A))) EXT_RES5 __attribute__((space(eds)));
__eds__ uint8_t __attribute__((noload, address(0x00800C))) EXT_RES6 __attribute__((space(eds)));
__eds__ uint8_t __attribute__((noload, address(0x00800E))) EXT_RES7 __attribute__((space(eds)));

__eds__ uint8_t __attribute__((noload, address(0x008010))) SCOREDISPLAY[8] __attribute__((space(eds)));

__eds__ uint8_t __attribute__((noload, address(0x008018))) BONUSDISPLAY[8] __attribute__((space(eds)));

void _update7Seg(uint16_t value, __eds__ uint8_t *board )
{
    const uint8_t BCDToSeg[] = { LCD_0, LCD_1, LCD_2, LCD_3, LCD_4, LCD_5, LCD_6, LCD_7, LCD_8, LCD_9, 
                                 LCD_A, LCD_B, LCD_C, LCD_D, LCD_E, LCD_F };
                                 
    const uint16_t decades[] = { 1000, 100, 10, 1, 0 };

    const uint16_t *d = decades;

    // Fast forward until a multiple of 10 less then value is found
    while (value < *d)
    {
        ++d;
        ++board;
        *board++ = LCD_BLANK;
    }

    if (value == 0)
    {
        --board;
        *board = LCD_0;
        return;
    }
    
    while (*d)
    {
      uint8_t digit = 0;

      while (value >= *d)
      {
        ++digit;
        value -= *d;
      }

      ++board;
      *board++ = BCDToSeg[digit];
      ++d;
    }
}

void UpdateScoreBoard(uint16_t value)
{
    _update7Seg(value, SCOREDISPLAY);
}

void UpdateBonusBoard(uint16_t value)
{
    _update7Seg(value, BONUSDISPLAY);
}

void ReadUserInputs()
{
  
}