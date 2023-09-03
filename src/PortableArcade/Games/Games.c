
#include "../../PixiPusher/PixiGFX.h"
#include "../PortableArcade.h"
#include "../Menu.h"

const Color GAMEOVER_COLORS[] = {
    { .R = 255, .G = 0, .B = 0, .A = 0xFF },
    { .R = 0, .G = 255, .B = 0, .A = 0xFF },
    { .R = 0, .G = 0, .B = 255, .A = 0xFF },
};

extern PixiGFX *graphics;
extern const PGfxFont Font1;
const char* STR_GAMEOVER = "Gameover";

MenuResult GameoverLoop(bool reset) {
    MenuResult result = { .MenuReturn = Continue, .NextDelay = 50 };

    static int textIndex = 0;
    static int pass = 0;
    static int textLength = 0;
    static bool round1 = true;

    if (reset) {
        textIndex = graphics->Matrix->Width;
        textLength = PG_GetTextLengthC(STR_GAMEOVER, &Font1);
        pass = 0;
        round1 = true;
    }

    if (--textIndex < (textLength * -1))
    {
        textIndex = graphics->Matrix->Width;
        if (++pass >= 3) {
            pass = 0;
        } else if (pass > 0) {
            round1 = false;
        }
    }

    PG_DrawTextC(graphics, STR_GAMEOVER, textIndex, 8, GAMEOVER_COLORS[pass], Black, &Font1);

    if (!round1 && GetDirection() != NO_DIRECTION)
    {
        result.MenuReturn = Exit;
    }

    return result;
}