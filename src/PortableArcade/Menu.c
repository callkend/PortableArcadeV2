

#include "../PixiPusher/PixiPixel.h"
#include "../PixiPusher/PixiMatrix.h"
#include "../PixiPusher/PixiGFX.h"
#include "../PixiPusher/Color.h"

#include "PortableArcade.h"
#include "Menu.h"

#include <string.h>

extern const PGfxFont Font1;

void RenderMenuLine(PixiMatrix *matrix, char *text, int line, Color color){

    char x = PG_GetTextLength(text, &Font1);
    x = 15 - (x >> 1);

    char y = (line * 8);

    PG_FillRectangle(matrix, 0, y, 32, y + 8, Black);

    PG_DrawText(matrix, text, x, y, color, Black, &Font1);     
}

void RenderMenu(Menu_t *menu, PixiMatrix *matrix){
    
    char p[MAX_MENU_NAME_LENGTH];
    const static Color color = { .R = 32, .G = 0, .B = 0, .A = 0xFF };

    if (menu->SubMenus == NULL){
        strcpy(p, "Error:");
        RenderMenuLine(matrix, p, 0, color);
        strcpy(p, "No Sub");
        RenderMenuLine(matrix, p, 1, color);
        
    } else {
        int i;
        int j = menu->ActiveSubMenuIndex - 1;
        
        for (i = 0; i < 3; ++i) {
            const char* name = menu->SubMenus[j++].Name;
            if (j > 0 && name != NULL) {
                strcpy(p, name);
                RenderMenuLine(matrix, p, i, color);
            } else {
                p[0] = 0x00;
                RenderMenuLine(matrix, p, i, color);
            }
        }
    }
}

void ProcessMenus(MenuState_t *state, PixiMatrix *matrix){
    if (state->ActiveLoop != NULL)
    {
        if (state->ActiveLoop() == 0){
            return;
        }
        
        state->ActiveLoop = NULL;
        RenderMenu(state->ActiveMenu, matrix);
    } else {
        static UserInput_t lastInput = { .AllBits = 0xFF };
        
        UserInput_t input = ReadUserInputs();

        // Only process the input if the state has changed
        if (input.AllBits != lastInput.AllBits)
        {
            lastInput.AllBits = input.AllBits;

            if (!input.JoyRight){
                // If the menu has a loop, then process the loop
                if (state->ActiveMenu->Loop != NULL) {
                    state->ActiveMenu->Setup();
                    state->ActiveLoop = state->ActiveMenu->Loop;
                } else {
                    state->ActiveMenu->SubMenus[state->ActiveMenu->ActiveSubMenuIndex].ParentMenu = state->ActiveMenu;

                    state->ActiveMenu = &state->ActiveMenu->SubMenus[state->ActiveMenu->ActiveSubMenuIndex];
                }
            } else if (!input.JoyLeft) {
                if (state->ActiveMenu->ParentMenu != NULL){
                    state->ActiveMenu = state->ActiveMenu->ParentMenu;
                }

            } else if (!input.JoyUp) {
                if (state->ActiveMenu->ActiveSubMenuIndex > 0){
                    --state->ActiveMenu->ActiveSubMenuIndex;
                }
            } else if (!input.JoyDown){
                if (state->ActiveMenu->SubMenus[state->ActiveMenu->ActiveSubMenuIndex + 1].Name[0] != '\x00'){
                    ++state->ActiveMenu->ActiveSubMenuIndex;
                }
            }
            
            RenderMenu(state->ActiveMenu, matrix);
        }
    }
}

