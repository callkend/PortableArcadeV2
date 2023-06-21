
#include <stdint.h>

#include "Menu.h"
#include "PortableArcade.h"

#define MAX_TEST_NUMBER 10000

int16_t testScore = 0;
int16_t testBonus = 0;

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

Menu_t testMenu[] = {
    DEFINE_MENU_FUNCTION("Score", 0x0000, ScoreBoardTest),    
    DEFINE_MENU_FUNCTION("Bonus", 0x0000, BonusBoardTest),
    DEFINE_MENU_FUNCTION("Both", 0x0000, BothBoardTest),
    
    DEFINE_EMPTY_MENU(),
};


