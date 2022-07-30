/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c
 
  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system intialization and application code development.
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

#include "mcc_generated_files/system.h"
#include "mcc_generated_files/usb/usb.h"

#include "PixiPusher/PixiPixel.h"
#include "PixiPusher/PixiMatrix.h"
#include "PixiPusher/PixiGFX.h"
#include "PixiPusher/Color.h"
#include "PixiPusher/setup.h"

#define LEDCount (768)

uint8_t DisplayArray[LEDCount * PixelSize];

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
    extern const PGfxFont Font1;

    char text[] = { 0, 0, 0, 0, 0 };


    PixiMatrix matrix = PM_Init(16, 16, DisplayArray, PixelMap);
    Color c = { .R = 8, .G = 2, .B = 0, .A = 0xFF };
    
    PP_SetAutoUpdate(true);

    while (1)
    {
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
                text[3] = text[2];
                text[2] = text[1];
                text[1] = text[0];
                text[0] = readBuf[0];
                
                PG_DrawText(&matrix, text, 0, 0, c, LowestWhite, &Font1);
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

