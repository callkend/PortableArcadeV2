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
#include "Color.h"

/********************************************************/

#define ByteRateMin 0x0010
#define MaxChannelCount 4

// Waveform Characteristics
volatile WS2812Settings pixelSettings = {       \
    .ByteRate = 18,                             \
    .BitWidth = 8,                              \
    .ZeroWidth = 4,                             \
    .LatchWidth = 0x1000,                       \
    .AutoUpdate = true,                         \
    .Updating = false,                          \
};

/**
 * Changes total width of a bit
 * @param bitWidth
 */
void PP_SetBitWidth(WS2812Settings* settings, unsigned char bitWidth) {
    settings->BitWidth = bitWidth;

    //Adjust the byte rate so that overruns don't happen
    unsigned char tempByte = (bitWidth << 1) + 2;
    settings->ByteRate = tempByte < ByteRateMin ? ByteRateMin : tempByte;
}

/**
 * Changes the zero bit width
 * @param zeroWidth
 */
void PP_SetZeroWidth(WS2812Settings* settings, unsigned char zeroWidth) {
    settings->ZeroWidth = ZeroWidthReg = zeroWidth;
}

/**
 * Changes the period between frames
 * @param latchWidth
 */
void PP_SetLatchWidth(WS2812Settings* settings, unsigned int latchWidth) {
    settings->LatchWidth = latchWidth;
}

/**
 * Changes if the display is updated automatically or manually
 * @param autoUpdate
 */
void PP_SetAutoUpdate(WS2812Settings* settings, bool autoUpdate) {
    settings->AutoUpdate = autoUpdate;
}


volatile PixiChannelSettings channelSettings[] = {
    { .Channel = 0 },
    { .Channel = 0 },
    { .Channel = 0 },
    { .Channel = 0 },
};

void _PP_PrepareChannelForUpdate(PixiChannelSettings *settings);
void _PP_Fill(PixiChannelSettings settings, Color c);
void _PP_PrepareChannelsForUpdate();

void PP_Init() {

    /****Port Setup****/
    TRISDbits.TRISD9 = 0;           // CLCAIN / CLCBIN
    ANSDbits.ANSD9 = 0;
    RPINR25bits.CLCINAR = 12;       // CLCINA to RD9 (RP4)
    RPINR25bits.CLCINBR = 12;       // CLCINB to RD9 (RP4)
            
    TRISDbits.TRISD11 = 0;          // REFO
    ANSDbits.ANSD11 = 0;
    RPOR6bits.RP12R = _RPOUT_REFO1; // REFO to RP12
        
    TRISBbits.TRISB8 = 0;           // SCCP4
    ANSBbits.ANSB8 = 0;
    RPOR4bits.RP8R = _RPOUT_OCM4;   // SCCP4 to RB9
    
    
    /****PWM Setup****/
    //PWM 4 for the SPI clock source
    CCP4PRH = 0x0000; //Period Registers
    CCP4PRL = pixelSettings.BitWidth;

    CCP4RAH = 0x0000; //Pulse Width
    CCP4RAL = pixelSettings.ZeroWidth;

    CCP4CON3H = 0x0030; //Active high

    CCP4CON1H = 0x8F00; //Interrupt every 8 pulses
    CCP4CON1L = 0x8005; //Enable Module in PWM Mode

    /****SPI Setup****/
    //Reference Clock Setup
    // REFOTRIML = 0x0000; //No Trim
    REFOCONH = 0x0000; //No divider
    REFOCONL = 0x9008; //Clock Source = REFI Pin

    /****TIMER 1 Setup****/
    PR1 = 0x000F; //Triggers each byte to be sent, then the delay between packets
    T1CON = 0x8010; //Enable Timer 1, 1:8, FOSC/2
}

void PP_InitChannel(uint8_t channel, uint8_t *array, uint16_t count, Color initialColor) {

    // Do nothing if an invalid channel is selected
    if (channel == 0 || channel > MaxChannelCount){
        return;
    }

    PixiChannelSettings channelSetting = {  \
        .Array = array,                     \
        .Count = count,                     \
        .Channel = channel,                 \
        };

    channelSettings[channel] = channelSetting;
    
    /****Init Memory****/
    _PP_Fill(channelSetting, initialColor);

    /**** SPI Setup ****/
    //Enable Module, Master Mode, Standard Mode, REFO Clock 0xC024
#define SPIxCON1L 0xC025

    /**** CLC Setup ****/

    /*****2-Input D-FlipFlop*****/
#define CLCxGLSH 0x5000 //Select the sources and the polarity
#define CLCxGLSL 0x6401
#define CLCxSEL 0x3301  //Sources: SCK (SDI), SDO, NULL, PWM (CLCINA)
#define CLCxCONH 0x000B //Gate 3 and 4 inverted
#define CLCxCONL 0x80A7 //Enable CLC, Enable Output, 4-Input AND

    /**** DMA Setup ****/
#define DMAINTx 0x6300 //Trigger Source = SCCP4
#define DMACHx 0x0173  //Enable Module, 8-Bit, One Shot

    switch (channel) {
        case 1:
            // Port 
            TRISDbits.TRISD1 = 0; // Channel Output
            ANSDbits.ANSD1 = 0;
            RPOR12bits.RP24R = _RPOUT_CLC1OUT; //CLC1OUT to RP24
    
            TRISDbits.TRISD2 = 0; // Channel Clock Loop Out
            ANSDbits.ANSD2 = 0;
            RPOR11bits.RP23R = _RPOUT_SCK1OUT; //SCK1 to RP23

            TRISDbits.TRISD3 = 1; // Channel Clock Loop In
            ANSDbits.ANSD3 = 0;
            RPINR20bits.SDI1R = 22; // SDI1 Input (RP22)

            // SPI
            SPI1CON1L = SPIxCON1L;

            // CLC
            CLC1GLSH = CLCxGLSH;
            CLC1GLSL = CLCxGLSL;

            CLC1SEL = CLCxSEL;

            CLC1CONH = CLCxCONH;
            CLC1CONL = CLCxCONL;

            // DMA
            DMADST0 = (unsigned int) &SPI1BUFL; //Setup destination pointer

            DMAINT0 = DMAINTx; //Trigger Source = SCCP4
            DMACH0 = DMACHx; //Enable Module, 8-Bit, One Shot
            break;

        case 2:
            // Port
            TRISDbits.TRISD0 = 0; // Channel Output
            ANSDbits.ANSD0 = 0;

            TRISBbits.TRISB5 = 0; // Channel Clock Out
            ANSBbits.ANSB5 = 0;
            RPOR9bits.RP18R = _RPOUT_SCK2OUT; //SCK2 to RP18

            TRISBbits.TRISB4 = 1; // Channel Clock In
            ANSBbits.ANSB4 = 0;
            RPINR22bits.SDI2R = 28; //SDI2 Input (RP28)

            // SPI
            SPI2CON1L = SPIxCON1L;

            // CLC
            CLC2GLSH = CLCxGLSH;
            CLC2GLSL = CLCxGLSL;

            CLC2SEL = CLCxSEL;

            CLC2CONH = CLCxCONH;
            CLC2CONL = CLCxCONL;

            // DMA
            DMADST1 = (unsigned int) &SPI2BUFL; //Setup destination pointer

            DMAINT1 = DMAINTx; //Trigger Source = SCCP4
            DMACH1 = DMACHx; //Enable Module, 8-Bit, One Shot

            break;

        case 3:
            // Port
            TRISDbits.TRISD10 = 0; // Channel Output
            ANSDbits.ANSD10 = 0;
            RPOR1bits.RP3R = _RPOUT_CLC2OUT; //CLC2OUT to RP3

            TRISBbits.TRISB2 = 0; // Channel clock out
            ANSBbits.ANSB2 = 0;
            RPOR6bits.RP13R = _RPOUT_SCK3OUT; //SCK3 to RP13

            TRISBbits.TRISB1 = 1; // Channel clock in
            ANSBbits.ANSB1 = 0;
            RPINR28bits.SDI3R = 1; //SDI3 Input (RP1)

            // SPI
            SPI3CON1L = SPIxCON1L;

            // CLC
            CLC3GLSH = CLCxGLSH;
            CLC3GLSL = CLCxGLSL;

            CLC3SEL = CLCxSEL;

            CLC3CONH = CLCxCONH;
            CLC3CONL = CLCxCONL;

            // DMA
            DMADST2 = (unsigned int) &SPI3BUFL; //Setup destination pointer

            DMAINT2 = DMAINTx; //Trigger Source = SCCP4
            DMACH2 = DMACHx; //Enable Module, 8-Bit, One Shot

            break;

        case 4:
            // Port
            TRISDbits.TRISD8 = 0; // Channel Output
            ANSDbits.ANSD8 = 0;

            TRISBbits.TRISB13 = 0; // Channel Clock Out
            ANSBbits.ANSB13 = 0;

            TRISFbits.TRISF1 = 1; // Channel Clock In
            ANSFbits.ANSELF1 = 0;

            // SPI
            SPI4CON1L = SPIxCON1L;

            // CLC
            CLC4GLSH = CLCxGLSH;
            CLC4GLSL = CLCxGLSL;

            CLC4SEL = CLCxSEL;

            CLC4CONH = CLCxCONH;
            CLC4CONL = CLCxCONL;

            // DMA
            DMADST3 = (unsigned int) &SPI4BUFL; //Setup destination pointer

            DMAINT3 = DMAINTx; //Trigger Source = SCCP4
            DMACH3 = DMACHx; //Enable Module, 8-Bit, One Shot
            break;
    }

    _PP_PrepareChannelForUpdate(&channelSetting);

}

void PP_SetPixel(uint8_t *pnt, uint16_t pixelIndex, Color c) {

    pnt += (pixelIndex * PixelSize);

    *pnt++ = c.G;
    *pnt++ = c.R;
    *pnt++ = c.B;
}

Color PP_GetPixel(uint8_t *pnt, uint16_t pixelIndex) {

    Color result = {
        .G = *pnt++,
        .R = *pnt++,
        .B = *pnt++,
    };

    return result;
}

void _PP_Fill(PixiChannelSettings settings, Color c)
{
    uint8_t *pnt = settings.Array;

    for (int i = 0; i < settings.Count; i += PixelSize) {
        *pnt++ = c.G;
        *pnt++ = c.R;
        *pnt++ = c.B;
    }
}

void PP_Fill(Color c) {

    for (int i = 0; i < MaxChannelCount; ++i)
    {
        _PP_Fill(channelSettings[i], c);
    }
}

void PP_Service() {
}

void PP_DisplayUpdate() {
    PP_WaitForDisplayUpdateFinished();
    
    PP_DisplayUpdateStart();
    PP_WaitForDisplayUpdateFinished();
}

void PP_DisplayUpdateStart() {

    pixelSettings.Updating = true;

    ByteRateReg = pixelSettings.ByteRate; //Setup to clock data into DMA

    // Get all the Channles that are setup going as close to each other as possible
    if (channelSettings[0].Channel){
        DMACH0bits.CHEN = true; //Re Enable the Channel
    }
    if (channelSettings[1].Channel){
        DMACH1bits.CHEN = true; //Re Enable the Channel
    }
    if (channelSettings[2].Channel){
        DMACH2bits.CHEN = true; //Re Enable the Channel
    }
    if (channelSettings[3].Channel){
        DMACH3bits.CHEN = true; //Re Enable the Channel
    }

    /****Interrupt Setup*****/
    IEC0bits.DMA0IE = false;
    IEC0bits.DMA1IE = false;
    IEC1bits.DMA2IE = false;
    IEC2bits.DMA3IE = false;

    uint8_t lastChannel = 0;
    for (int i = 0; i < MaxChannelCount; ++i)
    {
        uint8_t channel = channelSettings[i].Channel;
        if (channel)
        {
            lastChannel = channel;
        }
    }

    switch(lastChannel)
    {
        case 1:
            IFS0bits.DMA0IF = 0; //DMA Interrupt
            IEC0bits.DMA0IE = 1;
            break;

        case 2:
            IFS0bits.DMA1IF = 0; //DMA Interrupt
            IEC0bits.DMA1IE = 1;
            break;

        case 3:
            IFS1bits.DMA2IF = 0; //DMA Interrupt
            IEC1bits.DMA2IE = 1;
            break;

        case 4:
            IFS2bits.DMA3IF = 0; //DMA Interrupt
            IEC2bits.DMA3IE = 1;
            break;
    }
};

void PP_WaitForDisplayUpdateFinished() {

    while (pixelSettings.Updating);

};

void _PP_PrepareChannelForUpdate(PixiChannelSettings *settings)
{
    switch (settings->Channel)
    {
        case 1:
            DMASRC0 = (uint16_t) settings->Array; //Setup source pointer
            DMACNT0 = settings->Count * PixelSize; //Set up the counter
            break;

        case 2:
            DMASRC1 = (uint16_t) settings->Array; //Setup source pointer
            DMACNT1 = settings->Count * PixelSize; //Set up the counter
            break;

        case 3:
            DMASRC2 = (uint16_t) settings->Array; //Setup source pointer
            DMACNT2 = settings->Count * PixelSize; //Set up the counter
            break;

        case 4:
            DMASRC3 = (uint16_t) settings->Array; //Setup source pointer
            DMACNT3 = settings->Count * PixelSize; //Set up the counter
            break;
    }
}

void _PP_StartLatchTime()
{
    LatchWidthReg = pixelSettings.LatchWidth; //Should create an interrupt every 16mS
    IFS0bits.T1IF = false; //Timer 1 Setup
    IEC0bits.T1IE = true;
}

void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt() {
    IFS0bits.DMA0IF = false; //Clear the flag
    if (DMAINT0bits.DONEIF) {
        DMAINT0bits.DONEIF = false; //Clear the DONEIF

        DMACH0bits.CHEN = false; //Disable channel

        _PP_StartLatchTime();
        _PP_PrepareChannelsForUpdate();
    }
    if (DMAINT0bits.HALFIF) {
        DMAINT0bits.HALFIF = false;
    }
    if (DMAINT0bits.OVRUNIF) {
        DMAINT0bits.OVRUNIF = false;
    }
}

void __attribute__((interrupt, no_auto_psv)) _DMA1Interrupt() {
    IFS0bits.DMA1IF = false; //Clear the flag
    if (DMAINT1bits.DONEIF) {
        DMAINT1bits.DONEIF = false; //Clear the DONEIF

        DMACH1bits.CHEN = false; //Disable channel

        _PP_StartLatchTime();
        _PP_PrepareChannelsForUpdate();
    }
    if (DMAINT1bits.HALFIF) {
        DMAINT1bits.HALFIF = false;
    }
    if (DMAINT1bits.OVRUNIF) {
        DMAINT1bits.OVRUNIF = false;
    }
}

void __attribute__((interrupt, no_auto_psv)) _DMA2Interrupt() {
    IFS1bits.DMA2IF = false; //Clear the flag
    if (DMAINT2bits.DONEIF) {
        DMAINT2bits.DONEIF = false; //Clear the DONEIF

        DMACH2bits.CHEN = false; //Disable channel

        _PP_StartLatchTime();
        _PP_PrepareChannelsForUpdate();
    }
    if (DMAINT2bits.HALFIF) {
        DMAINT2bits.HALFIF = false;
    }
    if (DMAINT2bits.OVRUNIF) {
        DMAINT2bits.OVRUNIF = false;
    }
}

void __attribute__((interrupt, no_auto_psv)) _DMA3Interrupt() {
    IFS2bits.DMA3IF = false; //Clear the flag
    if (DMAINT3bits.DONEIF) {
        DMAINT3bits.DONEIF = false; //Clear the DONEIF

        DMACH3bits.CHEN = false; //Disable channel

        _PP_StartLatchTime();
        _PP_PrepareChannelsForUpdate();
    }
    if (DMAINT3bits.HALFIF) {
        DMAINT3bits.HALFIF = false;
    }
    if (DMAINT3bits.OVRUNIF) {
        DMAINT3bits.OVRUNIF = false;
    }
}

void _PP_PrepareChannelsForUpdate()
{
    for (int i = 0; i < MaxChannelCount; ++i)
    {
        PixiChannelSettings p = channelSettings[i];
        
        if (p.Channel)
        {
            _PP_PrepareChannelForUpdate(&p);
        }
    }
}

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt() {

    IEC0bits.T1IE = false;
    IFS0bits.T1IF = false; //Lower the flag

    pixelSettings.Updating = false;

    if (pixelSettings.AutoUpdate) {
        PP_DisplayUpdateStart();
    }
}
