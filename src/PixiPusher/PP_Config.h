/* 
 * File:   PP_Config.h
 * Author: callwyat
 *
 * Created on December 9, 2020, 6:45 PM
 */

#ifndef PP_CONFIG_H
#define	PP_CONFIG_H

#define InitRed 1
#define InitGreen 1
#define InitBlue 1

#define BitWidthReg CCP4PRL
#define ZeroWidthReg CCP4RAL
#define LatchWidthReg PR1
#define ByteRateReg PR1

#define PixelSize (3)
#define PixelCount (768)
#define LEDBufferSize (PixelCount * PixelSize)

#ifdef	__cplusplus
}
#endif

#endif	/* PP_CONFIG_H */

