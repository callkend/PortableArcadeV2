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

#include "PixiPusher/PixiPixel.h"
#include "PixiPusher/PixiMatrix.h"
#include "PixiPusher/PixiGFX.h"
#include "PixiPusher/Color.h"
#include "PixiPusher/setup.h"

#include "PortableArcade/PortableArcade.h"
#include "PortableArcade/Menu.h"

#define LEDCount (768)

uint8_t DisplayArray[LEDCount * PixelSize];

extern const PGfxFont Font1;

Menu_t snakeMenu[] = {
    DEFINE_MENU("Easy", NULL),    
    DEFINE_MENU("Hard", NULL),
    DEFINE_EMPTY_MENU(),
};

Menu_t configMenu[] = {
    DEFINE_MENU("Bright", NULL),
    DEFINE_EMPTY_MENU(),
};

Menu_t menu_1[] = {
    DEFINE_MENU("Snake", snakeMenu),
    DEFINE_MENU("Tetris", NULL),
    DEFINE_MENU("Config", configMenu),
    DEFINE_EMPTY_MENU(),
};


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

    Menu_t mainMenu = DEFINE_MENU("Main Menu", menu_1);
    MenuState_t menuState = {
        .ActiveMenu = &mainMenu,
        .ActiveLoop = NULL,
    };
     
#define TextLineLength 16
#define LineCount 3
    
    char text[LineCount][TextLineLength];
    
    for (int y = 0; y < LineCount; ++y)
    {
        for (int x = 0; x < LineCount; ++x)
        {
            text[y][x] = 0x00;
        }
    }
    
    char activeLine = 0;

    PixiMatrix matrix = PM_Init(32, 24, DisplayArray, PixelMap);
    Color color = { .R = 0, .G = 0, .B = 255, .A = 0xFF };
    
    PP_SetAutoUpdate(true);
    
    ResetArcade();
    
    RenderMenu(menuState.ActiveMenu, &matrix);

    while (1)
    {        
        ProcessMenus(&menuState, &matrix);
         
        if( USBGetDeviceState() < CONFIGURED_STATE )
        {
            continue;
        }

        if( USBIsDeviceSuspended() == true )
        {
            continue;
        }

        if( USBUSARTIsTxTrfReady() == true)
        {
            static unsigned char readBuf[CDC_DATA_OUT_EP_SIZE]; //Buffer to store the incoming USB Data
            unsigned int readCount;

            readCount = getsUSBUSART(readBuf, CDC_DATA_OUT_EP_SIZE);

            if (readCount > 0)
            {
                char c = readBuf[0];
                
                switch (c)
                {
                    case '\n':
                        break;
                        
                    case '\r':
                        if (++activeLine >= LineCount)
                        {
                            activeLine = 0;
                        }
                        break;
                    
                    case '\x7F':
                    case '\b':
                    {
                        char *p = text[activeLine];
                        
                        if (*p == 0x00)
                        {
                            if (activeLine > 0)
                            {
                                --activeLine;
                            }
                        }
                        else
                        {
                            while (*p)
                            {
                                ++p;
                            }

                            --p;
                            *p = 0x00;     
                        }

                    }
                        break;
                        
                    default:
                    {
                        char *p = text[activeLine];
                        
                        while (*p)
                        {
                            ++p;
                        }
                        
                        char length = p - text[activeLine];
                        
                        if (length < (TextLineLength - 1))
                        {
                            *p++ = c;
                            *p = 0x00;
                        }
                    }
                        break;
                }
                
                {
                    char *p = text[activeLine];
                    
                    char x = PG_GetTextLength(p, &Font1);
                    x = 15 - (x >> 1);
                
                    char y = (activeLine * 8);
                    
                    PG_FillRectangle(&matrix, 0, y, 32, y + 8, Black);
                           
                    PG_DrawText(&matrix, p, x, y, color, Black, &Font1);        
                }

            }
        }

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

