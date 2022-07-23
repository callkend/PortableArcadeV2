/*********************************************************
 * Name: Wyatt J Callister
 * Date: February 1, 2017
 * 
 * Description: PixiPusher 4.0 is a continuation of the PixiPuhser programs. This
 * program takes a new the basic ideas of the PixiPuhser 4.0 and applies them to
 * a PIC24. This revision changes the data input type for USART to USB increasing
 * eliminating the the bottle neck that was imposed by USAERT. Additionally this
 * PIC allows for 4 signals to be generated using by using the CLC Modules with 
 * a PWM and an SPI module, allowing for up to 4095 LEDs to be updated at 30fps
 * 
*********************************************************/
    
#ifndef PIXI_PUSHER_H
#define	PIXI_PUSHER_H
    
    #include "PP_Config.h"
    #include "Color.h"
    #include <stdbool.h>
    #include <stdint.h>

    #define PixelSize (3)

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
    } WS2812Settings;
    
    typedef struct
    {
        uint8_t *Array;

        uint16_t Count;

        WS2812Settings Settings;

        uint8_t Channel;

        bool AutoUpdate;

    } PixiPixelSettings;

    /****Program Declarations********************************/

    /**
     * Initializes the PixiPusher Routines and starts transmitting the screen
     */
    PixiPixelSettings PP_Init(uint16_t count, Color initialColor);

    /**
     * Called periodically from main to keep the screen alive
     */
    void PP_Service(PixiPixelSettings *settings);

    
    // Waveform Characteristics    
    /**
     * Changes total width of a bit
     * @param bitWidth
     */
    void PP_SetBitWidth(PixiPixelSettings *settings, unsigned char bitWidth);
    
    /**
     * Changes the zero bit width
     * @param zeroWidth
     */
    void PP_SetZeroWidth(PixiPixelSettings *settings, unsigned char zeroWidth);
    
    /**
     * Changes the period between frames
     * @param latchWidth
     */
    void PP_SetLatchWidth(PixiPixelSettings *settings, unsigned int latchWidth);
    
    void PP_Fill(PixiPixelSettings *settings, Color c);
    
    /****End Program Declarations****************************/

#endif
