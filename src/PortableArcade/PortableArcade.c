
#include <stdbool.h>
#include <stdint.h>

#include "PortableArcade.h"
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

ReadUserInputsHandle AlternateReadUserInput;

void _update7Seg(uint16_t value, __eds__ uint8_t *board) {
    const uint8_t BCDToSeg[] = {LCD_0, LCD_1, LCD_2, LCD_3, LCD_4, LCD_5, LCD_6, LCD_7, LCD_8, LCD_9,
        LCD_A, LCD_B, LCD_C, LCD_D, LCD_E, LCD_F};

    const uint16_t decades[] = {1000, 100, 10, 1, 0};

    const uint16_t *d = decades;

    // Fast forward until a multiple of 10 less then value is found
    while (value < *d) {
        ++d;
        ++board;
        *board++ = LCD_BLANK;
    }

    if (value == 0) {
        --board;
        *board = LCD_0;
        return;
    }

    while (*d) {
        uint8_t digit = value / *d;

        value -= (*d++ * digit);

        ++board;
        *board++ = BCDToSeg[digit];
    }
}

void _clear7Seg( __eds__ uint8_t *board) {
    ++board;
    *board++ = LCD_BLANK;
    ++board;
    *board++ = LCD_BLANK;
    ++board;
    *board++ = LCD_BLANK;
    ++board;
    *board++ = LCD_BLANK;
}

void ClearScoreBoard() {
    _clear7Seg(SCOREDISPLAY);
}

void UpdateScoreBoard(uint16_t value) {
    _update7Seg(value, SCOREDISPLAY);
}

void ClearBonusBoard() {
    _clear7Seg(SCOREDISPLAY);
}

void UpdateBonusBoard(uint16_t value) {
    _update7Seg(value, BONUSDISPLAY);
}

void ResetArcade(void) {
    ClearScoreBoard();
    ClearBonusBoard();
}

UserInput_t ReadUserInputs(void) {

    static uint8_t lastBits = 0x00;

    UserInput_t result = {
        .AllBits = ~EXTNIN1,
        .LastBits = lastBits,
    };

    lastBits = result.AllBits;

    if (result.AllBits == 0x00 && AlternateReadUserInput != 0x00)
    {
        result = AlternateReadUserInput();
    }

    return result;
}

void SetAlternateUserInputHandle(ReadUserInputsHandle handle)
{
    AlternateReadUserInput = handle;
}

Direction_e GetDirection(void) {
    
    UserInput_t userInput = ReadUserInputs();
    
    if (userInput.JoyLeft) {
        return LEFT;
    } else if (userInput.JoyRight) {
        return RIGHT;
    } else if (userInput.JoyUp) {
        return UP;
    } else if (userInput.JoyRight) {
        return DOWN;
    } else {
        return NO_DIRECTION;
    }
}

bool SDCardPresent(void) {
    return (EXTNIN2 & 0x01) == 0;
}

void SetSDCardCS(bool value) {
    EXTNOUT1 = value ? 0x02 : 0x00;
}

void SetXBeeCS(bool value) {
    EXTNOUT1 = value ? 0x01 : 0x00;
}
