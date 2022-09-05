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
    
#ifndef PIXI_PUSHER_H
#define	PIXI_PUSHER_H
    
    #include "Color.h"
    #include <stdbool.h>
    #include <stdint.h>

    #define PixelSize (3)

    #define BitWidthReg CCP4PRL
    #define ZeroWidthReg CCP4RAL
    #define LatchWidthReg PR1
    #define ByteRateReg PR1

    typedef struct
    {
        /** Changes how often bytes are sent */
        unsigned char ByteRate;
        
        /** Changes total width of a bit */
        unsigned char BitWidth;
        
        /** Changes the zero bit width */
        unsigned char ZeroWidth;
        
        /** Changes the period between frames */
        unsigned int LatchWidth;

        /** Indicates if the screen updates asynchronously **/
        bool AutoUpdate;

        /** Indicates if the screen is currently updating **/
        bool Updating;
    } WS2812Settings;
    
    typedef struct
    {
        uint8_t *Array;

        uint16_t Count;

        uint8_t Channel;

    } PixiChannelSettings;

    /****Program Declarations********************************/
    
    // Waveform Characteristics    
    /**
     * Changes total width of a bit
     * @param bitWidth
     */
    void PP_SetBitWidth(unsigned char bitWidth);
    
    /**
     * Changes the zero bit width
     * @param zeroWidth
     */
    void PP_SetZeroWidth(unsigned char zeroWidth);
    
    /**
     * Changes the period between frames
     * @param latchWidth
     */
    void PP_SetLatchWidth(unsigned int latchWidth);
    
   

    /**
     * Initializes the PixiPusher Routines and starts transmitting the screen
     */
    void PP_Init();

    void PP_InitChannel(uint8_t channel, uint8_t *array, uint16_t count, Color initialColor);
    
    /**
     * Called periodically from main to keep the screen alive
     */
    void PP_Service();

    void PP_SetAutoUpdate(bool value);

    void PP_DisplayUpdate();
    
    void PP_DisplayUpdateStart();
    
    void PP_WaitForDisplayUpdateFinished();
    
    void PP_Fill(Color c);

    Color PP_GetPixel(uint8_t *pnt, uint16_t pixelIndex);

    void PP_SetPixel(uint8_t *pnt, uint16_t pixelIndex, Color c);
    
    /****End Program Declarations****************************/

#endif
