/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c
 
  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.95-b-SNAPSHOT
        Device            :  PIC24FJ128GB406
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36
        MPLAB 	          :  MPLAB X v5.10
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include <stdint.h>
#include <string.h>

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/usb/usb.h"
#include "mcc_generated_files/tmr2.h"

#include "PixiPusher/PixiPixel.h"
#include "PixiPusher/PixiMatrix.h"
#include "PixiPusher/PixiGFX.h"
#include "PixiPusher/Color.h"
#include "PixiPusher/setup.h"

#include "PortableArcade/PortableArcade.h"
#include "PortableArcade/Menu.h"

#include "PortableArcade/Games/Tetris.h"
#include "PortableArcade/Games/Snake.h"

#define LEDCount (768)

uint8_t DisplayArray[LEDCount * PixelSize];

extern const PGfxFont Font1;

PixiGFX *graphics;

MenuState_t menuState;

typedef int (*TaskHandle)(void);

typedef struct
{
    TaskHandle Handle;
    uint8_t Tick;
    uint8_t Top;
} Task_t;

#define DEFINE_TASK(handle, tick, top) \
    {                                  \
        .Handle = handle,              \
        .Tick = tick,                  \
        .Top = top                     \
    }

MenuResult ManuallyAdjustBrightness(PixiGFX * graphics);

Menu_t snakeMenu[] = {
    DEFINE_MENU_FUNCTION("Easy", snakeSetup, snakeLoop),    
    DEFINE_MENU_FUNCTION("Hard", snakeSetup, snakeLoop),
    DEFINE_EMPTY_MENU(),
};

Menu_t configMenu[] = {
    DEFINE_MENU_FUNCTION("Bright", NULL, ManuallyAdjustBrightness),
    DEFINE_EMPTY_MENU(),
};

Menu_t mainMenuSubs[] = {
    DEFINE_MENU("Snake", snakeMenu),
    DEFINE_MENU_FUNCTION("Tetris", tetrisSetup, tetrisLoop),
    DEFINE_MENU("Config", configMenu),
    DEFINE_EMPTY_MENU(),
};

MenuResult ManuallyAdjustBrightness(PixiGFX * graphics) {

    UserInput_t input = ReadUserInputs();
    MenuResult result = { .MenuReturn = Continue, .NextDelay = 20 };

    if (input.JoyInputs != input.LastJoyInputs) {
        
        if (input.JoyUp) {
            if (graphics->Brightness < 7) {
                PG_SetBrightness(graphics, graphics->Brightness + 1);
            }

        } else if (input.JoyDown) {
            if (graphics->Brightness > 1) {
                PG_SetBrightness(graphics, graphics->Brightness - 1);
            }

        } else if (input.JoyLeft) {
            result.MenuReturn = Exit;
            return result;
        }

        PG_DrawNumber(graphics, graphics->Brightness, 
                (graphics->Matrix->Width >> 1) - 3,
                (graphics->Matrix->Height >> 1) - 4,
                White, Black, &Font1);
    }

    return result;
}

int MenuTask(void) {

    return ProcessMenus(&menuState, graphics);
}

Task_t Tasks[] = {
    DEFINE_TASK(MenuTask, 0, 20),
};

uint8_t tasksSize = sizeof(Tasks) / sizeof(Tasks[1]);
bool tasksTicked = false;

void Timer2_Tick(void) {

    for (uint8_t i = 0; i < tasksSize; ++i)
    {
        ++Tasks[i].Tick;
    }

    tasksTicked = true;
}

void ProcessTasks(void)
{
    if (tasksTicked) {
        tasksTicked = false;

        Task_t *task;

        for (uint8_t i = 0; i < tasksSize; ++i)
        {
            task = &Tasks[i];

            if (task->Tick >= task->Top)
            {
                task->Tick = 0;
                int nextTop = task->Handle();

                if (nextTop > 0) {
                    task->Top = nextTop;
                }
            }
        }
    }
}

UserInput_t ReadUSBUserInputs(void)
{
    UserInput_t result = { .AllBits = 0x00 };

    if( USBGetDeviceState() < CONFIGURED_STATE )
    {
        return result;
    }

    if( USBIsDeviceSuspended() == true )
    {
        return result;
    }

    if( USBUSARTIsTxTrfReady() == true)
    {
        static unsigned char readBuf[CDC_DATA_OUT_EP_SIZE]; //Buffer to store the incoming USB Data
        unsigned int readCount;

        readCount = getsUSBUSART(readBuf, CDC_DATA_OUT_EP_SIZE);

        if (readCount > 0)
        {
            switch (readBuf[0])
            {
                case 'a':
                case 'A':
                {
                    result.JoyLeft = 1;
                    break;
                }
                    
                case 's':
                case 'S':
                {
                    result.JoyDown = 1;
                    break;
                }
                
                case 'd':
                case 'D':
                {
                    result.JoyRight = 1;
                    break;
                }

                case 'w':
                case 'W':
                {
                    result.JoyUp = 1;
                    break;
                }
                
                // Handle escape codes
                case '\x1b':
                {
                    if (readBuf[1] == '[')
                    {
                        switch (readBuf[2])
                        {
                            case 'D':
                            {
                                result.JoyLeft = 1;
                                break;
                            }

                            case 'B':
                            {
                                result.JoyDown = 1;
                                break;
                            }

                            case 'C':
                            {
                                result.JoyRight = 1;
                                break;
                            }

                            case 'A':
                            {
                                result.JoyUp = 1;
                                break;
                            }
                        }
                    }
                }
                    
                default:
                {
                    break;
                }
            }
        }
    }

    return result;
}

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    
    PP_Init();
    PP_InitChannel(4, DisplayArray, LEDCount, LowestWhite);
    
    Setup();

    extern uint16_t PixelMap[];

    Menu_t mainMenu = DEFINE_MENU("Main Menu", mainMenuSubs);
    menuState.ActiveMenu = &mainMenu;
    menuState.ActiveLoop = NULL;
     
    PixiMatrix matrix = PM_Init(32, 24, DisplayArray, PixelMap);
    PixiGFX g = PG_Init(&matrix);
    graphics = &g;
    PG_SetBrightness(graphics, 3);
    
    PP_SetAutoUpdate(true);
    
    ResetArcade(false);
    
    RenderMenu(menuState.ActiveMenu, graphics);

    TMR2_SetInterruptHandler(&Timer2_Tick);

    SetAlternateUserInputHandle(&ReadUSBUserInputs);

    while (1)
    {
        ProcessTasks();

        // PP_Service();
        CDCTxService();
    }

    return 1;
}

// Button Interrupt
void __attribute__((interrupt,no_auto_psv)) _CNInterrupt(){
    
    // Clear the flag
    IFS1bits.CNIF = 0;
}

/**
 End of File
*/

