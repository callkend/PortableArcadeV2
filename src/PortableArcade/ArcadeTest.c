
#include <stdint.h>

#include "Menu.h"
#include "PortableArcade.h"

#include "../PixiPusher/PixiGFX.h"

#define MAX_TEST_NUMBER 10000
extern const PGfxFont Font1;

int16_t testScore = 0;
int16_t testBonus = 0;

extern unsigned char Read();

int16_t IncrementTestNumber(int16_t number) {
    if (++number >= MAX_TEST_NUMBER) {
        number = 0;
    }

    return number;
}

int16_t DecrementTestNumber(int16_t number) {
    if (--number < 0) {
        number = (MAX_TEST_NUMBER - 1);
    }

    return number;
}

MenuResult ScoreBoardTest(PixiGFX * graphics) {

    UserInput_t input = ReadUserInputs();
    MenuResult result = { .MenuReturn = Continue, .NextDelay = 20 };

    if (input.JoyInputs != input.LastJoyInputs) {
        
        if (input.JoyUp) {
            testScore = IncrementTestNumber(testScore);
        } else if (input.JoyDown) {
            testScore = DecrementTestNumber(testScore);
        } else if (input.JoyLeft) {
            result.MenuReturn = Exit;
            return result;
        }

        UpdateScoreBoard(testScore);
    }

    return result;
}

MenuResult BonusBoardTest(PixiGFX * graphics) {

    UserInput_t input = ReadUserInputs();
    MenuResult result = { .MenuReturn = Continue, .NextDelay = 20 };

    if (input.JoyInputs != input.LastJoyInputs) {
        
        if (input.JoyUp) {
            testBonus = IncrementTestNumber(testBonus);
        } else if (input.JoyDown) {
            testBonus = DecrementTestNumber(testBonus);
        } else if (input.JoyLeft) {
            result.MenuReturn = Exit;
            return result;
        }

        UpdateBonusBoard(testBonus);
    }

    return result;
}

MenuResult BothBoardTest(PixiGFX * graphics) {

    UserInput_t input = ReadUserInputs();
    MenuResult result = { .MenuReturn = Continue, .NextDelay = 20 };

    if (input.JoyInputs != input.LastJoyInputs) {
        
        if (input.JoyUp) {
            testBonus = IncrementTestNumber(testBonus);
            testScore = IncrementTestNumber(testScore);
        } else if (input.JoyDown) {
            testBonus = DecrementTestNumber(testBonus);
            testScore = DecrementTestNumber(testScore);
        } else if (input.JoyLeft) {
            result.MenuReturn = Exit;
            return result;
        }

        UpdateScoreBoard(testScore);
        ReadUserInputs();
        UpdateBonusBoard(testBonus);
        ReadUserInputs();
    }

    return result;
}


Menu_t testScoreBoardMenu[] = {
    DEFINE_MENU_FUNCTION("Score", 0x0000, ScoreBoardTest),    
    DEFINE_MENU_FUNCTION("Bonus", 0x0000, BonusBoardTest),
    DEFINE_MENU_FUNCTION("Both", 0x0000, BothBoardTest),
    
    DEFINE_EMPTY_MENU(),
};

MenuResult TextTest(PixiGFX * graphics){

    MenuResult result = { .MenuReturn = Continue, .NextDelay = 20 };

    static int cursorX = 0;
    static int cursorY = 8;

    const unsigned int MAX_TEXT_LENGTH = 12;

    static char text[13];
    static int textLength  = 0;

    char c = Read();
    switch (c)
    {
        case -1:
        {
            return result; // Nothing new to read
        }
        break;

        // Handle escape codes
        case '\x1b':
        {
            if (Read() == '[')
            {
                switch (Read())
                {
                    case 'D':   // Left Arrow
                    {
                        --cursorX;
                        break;
                    }

                    case 'B':   // Down Arrow
                    {
                        ++cursorY;
                        break;
                    }

                    case 'C':   // Right Arrow
                    {
                        ++cursorX;
                        break;
                    }

                    case 'A':   // Up Arrow
                    {
                        --cursorY;
                        break;
                    }
                }
            }
            else
            {
                result.MenuReturn = Exit;
            }
        }

        break;
        
        case '\x08': // Backspace
            if (textLength > 0)
            {
                --textLength;
            }
            
            break;
            
        default:
        {
            if (textLength < MAX_TEXT_LENGTH)
            {
                text[textLength] = c;
                text[++textLength] = '\x00';
            }
        }
        break;
    }

    PG_Fill(graphics, Black);
    PG_DrawText(graphics, text, cursorX, cursorY, Green, Black, &Font1);
                    
    return result;
}

Menu_t testMenu[] = {
    DEFINE_MENU("Score", testScoreBoardMenu),
    DEFINE_MENU_FUNCTION("Text", 0x0000, TextTest),    

    DEFINE_EMPTY_MENU(),
};


