/* 
 * File:   SDCard.h
 * Author: callwyat
 *
 * Created on August 10, 2022, 9:43 PM
 */

#ifndef SDCARD_H
#define	SDCARD_H

#ifdef	__cplusplus
extern "C" {
#endif

    #include <stdint.h>
    
    typedef union SDCardCommand_t
    {
        struct
        {
            uint8_t Command;
            uint8_t Arg3;    
            uint8_t Arg2;
            uint8_t Arg1;
            uint8_t Arg0;
            uint8_t CRC;
        };
        struct
        {
            uint8_t Array[6];
        };
    } SDCardCommand_t;
    
#define SDCommand(command, arg3, arg2, arg1, arg0, crc) \
{                                                       \
    .Command = command,                                 \
    .Arg3 = arg3,                                       \
    .Arg2 = arg2,                                       \
    .Arg1 = arg1,                                       \
    .Arg0 = arg0,                                       \
    .CRC = crc                                          \
}

    uint8_t SDCardInit(void);



#ifdef	__cplusplus
}
#endif

#endif	/* SDCARD_H */

