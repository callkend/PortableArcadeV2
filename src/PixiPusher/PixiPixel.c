/*********************************************************
 * Name: Wyatt J Callister
 * Date: February 1, 2017
 * 
 * Description: PixiPusher 4.0 is a continuation of the PixiPusher programs. This
 * program takes a new the basic ideas of the PixiPusher 4.0 and applies them to
 * a PIC24. This revision changes the data input type for USART to USB increasing
 * eliminating the the bottle neck that was imposed by USART. Additionally this
 * PIC allows for 4 signals to be generated using by using the CLC Modules with 
 * a PWM and an SPI module, allowing for up to 4095 LEDs to be updated at 30fps
 * 
*********************************************************/
/** INCLUDES ********************************************/
#include <xc.h>
#include <stdlib.h>
#include <stdbool.h>

#include "PixiPixel.h"
#include "PP_Config.h"

/********************************************************/

#define ByteRateMin 0x0010

#define NumberOfChannels (1)

#define PixelsPerChannel (PixelCount / NumberOfChannels)
#define BytesPerChannel (PixelsPerChannel * PixelSize)
    
uint8_t LEDRAM[LEDBufferSize]; //Stores all LED data in on big array

const int LEDSPNT1 = ((unsigned int)&LEDRAM + BytesPerChannel * 0);

// Waveform Characteristics
WS2812Settings pixelSettings = {    \
    .ByteRate = 18,                 \
    .BitWidth = 8,                  \
    .ZeroWidth = 4,                 \
    .LatchWidth = 0x1000            \
};

/**
 * Changes total width of a bit
 * @param bitWidth
 */
void PP_SetBitWidth(unsigned char bitWidth){
    pixelSettings.BitWidth = bitWidth;
    
    //Adjust the byte rate so that overruns don't happen
    unsigned char tempByte = (bitWidth << 1) + 2;
    pixelSettings.ByteRate = tempByte < ByteRateMin ? ByteRateMin : tempByte;
}

/**
 * Changes the zero bit width
 * @param zeroWidth
 */
void PP_SetZeroWidth(unsigned char zeroWidth){
    pixelSettings.ZeroWidth = ZeroWidthReg = zeroWidth;
}

/**
 * Changes the period between frames
 * @param latchWidth
 */
void PP_SetLatchWidth(unsigned int latchWidth){
    pixelSettings.LatchWidth = latchWidth;
}
    
void PP_Init(void)
{
    /****Init Memory****/
    unsigned char *pnt = &LEDRAM[0];
    
    for (int i = 0; i < LEDBufferSize; i += 3)
    {
        *pnt++ = InitGreen;
        *pnt++ = InitRed;
        *pnt++ = InitBlue;
    }

    /****Port Setup****/
    ANSB = 0x10C8;  //Analog setup
    TRISB = 0x7ADB; //RP9 = Output, all others are inputs
    PORTB = 0xFFDB;

    ANSD = 0xF080;
    TRISD = 0xF2C8;

    ANSE = 0xFF00; //EPMP Port Pins
    TRISE = 0xFF00;

    ANSF = 0xFF5B;
    TRISF = 0xFFDF;

    ANSG = 0XFF3F;
    TRISG = 0xFEBF;
    PORTG = 0xFEBF;

    //Peripheral Pin Setup
    //Inputs
    RPINR25bits.CLCINAR = 12; //CLCINA to RD9 (RP4)
    RPINR25bits.CLCINBR = 12; //CLCINB to RD9 (RP4)

    //Outputs
    RPOR6bits.RP12R = _RPOUT_REFO1; //REFO to RP12

    RPOR12bits.RP24R = _RPOUT_CLC1OUT; //CLC1OUT to RP29
    RPOR1bits.RP3R = _RPOUT_CLC2OUT;   //CLC2OUT to RP3

    RPOR4bits.RP8R = _RPOUT_OCM4; //SCCP4 to RB9

    //Lock Registers
    __asm__ ("MOV    #OSCCON, w1 \n"
        "MOV    #0x46, w2   \n"
        "MOV    #0x57, w3   \n"
        "MOV.b  w2, [w1]    \n"
        "MOV.b  w3, [w1]    \n"
        "BSET   OSCCON, #6");

    /****SPI Setup****/
    //Reference Clock Setup
    // REFOTRIML = 0x0000; //No Trim
    REFOCONH = 0x0000;  //No divider
    REFOCONL = 0x9008;  //Clock Source = REFI Pin

    //SPI Configuration
    //Enable Module, Master Mode, Standard Mode, REFO Clock 0xC024
    SPI4CON1L = 0xC025; 

    /****PWM Setup****/
    //PWM 4 for the SPI clock source
    CCP4PRH = 0x0000; //Period Registers
    CCP4PRL = pixelSettings.BitWidth;

    CCP4RAH = 0x0000; //Pulse Width
    CCP4RAL = pixelSettings.ZeroWidth;

    CCP4CON3H = 0x0030; //Active high

    CCP4CON1H = 0x8F00; //Interrupt every 8 pulses
    CCP4CON1L = 0x8005; //Enable Module in PWM Mode

/****CLC Setup****/

/*****2-Input D-FlipFlop*****/
#define CLCxGLSH 0x5000 //Select the sources and the polarity
#define CLCxGLSL 0x6401
#define CLCxSEL 0x3301  //Sources: SCK (SDI), SDO, NULL, PWM (CLCINA)
#define CLCxCONH 0x000B //Gate 3 and 4 inverted
#define CLCxCONL 0x80A7 //Enable CLC, Enable Output, 4-Input AND

    /****CLC Setup****/
    //  4
    CLC4GLSH = CLCxGLSH;
    CLC4GLSL = CLCxGLSL;

    CLC4SEL = CLCxSEL;

    CLC4CONH = CLCxCONH;
    CLC4CONL = CLCxCONL;

    /****DMA Setup****/
    DMAL = 0x0000; //Setup addressable space
    DMAH = 0x7FFF;
    DMACON = 0x8000; //Enable the DMA Modules

    //DMA5      This channel is used for USB Transfers
    DMAINT5 = 0x0000; //No Trigger (ALU commands DMA to start)
    DMACH5 = 0x0059;  //Enable Module, 8-Bit, Continuous
    
    /***Output DMAs***/
    //DMA0
    DMADST0 = (unsigned int)&SPI4BUFL; //Setup destination pointer
    DMASRC0 = LEDSPNT1;                //Setup source pointer
    DMACNT0 = BytesPerChannel;         //Set up the counter for 1024 pixels

    DMAINT0 = 0x6300; //Trigger Source = SCCP4
    DMACH0 = 0x0173;  //Enable Module, 8-Bit, One Shot

    /****TIMER 1 Setup****/
    PR1 = 0x000F;   //Triggers each byte to be sent, then the delay between packets
    T1CON = 0x8010; //Enable Timer 1, 1:8, FOSC/2

    /****Interrupt Setup*****/
    IFS0bits.DMA0IF = 0; //DMA Interrupt
    IEC0bits.DMA0IE = 1;
}

void PP_Service(void)
{
}

void PP_UpdateDisplay(void)
{
    DMACH0bits.CHEN = true; //Re Enable the Channel
};

void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt()
{
    IFS0bits.DMA0IF = false; //Clear the flag
    if (DMAINT0bits.DONEIF)
    {
        DMAINT0bits.DONEIF = false; //Clear the DONEIF

        DMACH0bits.CHEN = false;   //Disable channel
        DMASRC0 = LEDSPNT1;        //Setup source pointer
        DMACNT0 = BytesPerChannel; //Set up the counter for 1024 pixels

        LatchWidthReg = pixelSettings.LatchWidth; //Should create an interrupt every 16mS
        IFS0bits.T1IF = false;      //Timer 1 Setup
        IEC0bits.T1IE = true;
    }
    if (DMAINT0bits.HALFIF)
    {
        DMAINT0bits.HALFIF = false;
    }
    if (DMAINT0bits.OVRUNIF)
    {
        DMAINT0bits.OVRUNIF = false;
    }
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt()
{
    IEC0bits.T1IE = false;
    IFS0bits.T1IF = false; //Lower the flag

    PP_UpdateDisplay();
    
    ByteRateReg = pixelSettings.ByteRate; //Setup to clock data into DMA
}
