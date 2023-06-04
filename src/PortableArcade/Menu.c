

#include "../PixiPusher/PixiGFX.h"
#include "../PixiPusher/Color.h"

#include "PortableArcade.h"
#include "Menu.h"

#include <string.h>

extern const PGfxFont Font1;

void RenderMenuLine(PixiGFX *graphics, char *text, int line, Color color){

    char x = PG_GetTextLength(text, &Font1);
    x = 15 - (x >> 1);

    char y = (line * 8);

    PG_FillRectangle(graphics, 0, y, 32, y + 8, Black);

    PG_DrawText(graphics, text, x, y, color, Black, &Font1);     
}

void RenderMenu(Menu_t *menu, PixiGFX *graphics){
    
    char p[MAX_MENU_NAME_LENGTH];

    if (menu->SubMenus == NULL){
        strcpy(p, "Error:");
        RenderMenuLine(graphics, p, 0, Orange);
        strcpy(p, "No Sub");
        RenderMenuLine(graphics, p, 1, Orange);
        strcpy(p, "");
        RenderMenuLine(graphics, p, 2, Orange);
        
    } else {
        int i;
        int j = menu->ActiveSubMenuIndex - 1;
        
        for (i = 0; i < 3; ++i) {
            const char* name = menu->SubMenus[j++].Name;
            if (j > 0 && name != NULL) {
                strcpy(p, name);
                RenderMenuLine(graphics, p, i, i == 1 ? Blue : Red);
            } else {
                p[0] = 0x00;
                RenderMenuLine(graphics, p, i, Red);
            }
        }
    }
}

void ProcessMenus(MenuState_t *state, PixiGFX *graphics){
    if (state->ActiveLoop != NULL)
    {
        MenuReturn returnCode = state->ActiveLoop(graphics);
        if (returnCode == Continue){
            return;
        }
        
        state->ActiveLoop = NULL;
        RenderMenu(state->ActiveMenu, graphics);
    } else {        
        UserInput_t input = ReadUserInputs();

        // Only process the input if the state has changed
        if (input.JoyInputs != input.LastJoyInputs)
        {
            if (input.JoyRight){
                // If the menu has a loop, then process the loop
                Menu_t selectedSubMenu = state->ActiveMenu->SubMenus[state->ActiveMenu->ActiveSubMenuIndex];
                if (selectedSubMenu.Loop != NULL) {
                    if (selectedSubMenu.Setup != NULL){
                        selectedSubMenu.Setup(graphics);
                    }
                    state->ActiveLoop = selectedSubMenu.Loop;

                    PG_Fill(graphics, Black);
                    return;
                } else {
                    state->ActiveMenu->SubMenus[state->ActiveMenu->ActiveSubMenuIndex].ParentMenu = state->ActiveMenu;

                    state->ActiveMenu = &state->ActiveMenu->SubMenus[state->ActiveMenu->ActiveSubMenuIndex];
                }
            } else if (input.JoyLeft) {
                if (state->ActiveMenu->ParentMenu != NULL){
                    state->ActiveMenu = state->ActiveMenu->ParentMenu;
                }

            } else if (input.JoyUp) {
                if (state->ActiveMenu->ActiveSubMenuIndex > 0){
                    --state->ActiveMenu->ActiveSubMenuIndex;
                }
            } else if (input.JoyDown){
                if (state->ActiveMenu->SubMenus[state->ActiveMenu->ActiveSubMenuIndex + 1].Name[0] != '\x00'){
                    ++state->ActiveMenu->ActiveSubMenuIndex;
                }
            }
            
            RenderMenu(state->ActiveMenu, graphics);
        }
    }
}

