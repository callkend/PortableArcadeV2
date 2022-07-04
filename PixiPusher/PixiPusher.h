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
    #include <stdint.h>

    extern uint8_t LEDRAM[LEDBufferSize];

    /****Program Declarations********************************/

    /**
     * Initializes the PixiPusher Routines and starts transmitting the screen
     */
    void PP_Setup(void);

    /**
     * Called periodically from main to keep the screen alive
     */
    void PP_Service(void);

    
    // Waveform Characteristics    
    /**
     * Changes total width of a bit
     * @param bitWidth
     */
    void SetBitWidth(unsigned char bitWidth);
    
    /**
     * Changes the zero bit width
     * @param zeroWidth
     */
    void SetZeroWidth(unsigned char zeroWidth);
    
    /**
     * Changes the period between frames
     * @param latchWidth
     */
    void SetLatchWidth(unsigned int latchWidth);
    
    /****End Program Declarations****************************/

#endif
