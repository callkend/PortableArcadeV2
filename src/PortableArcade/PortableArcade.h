/* 
 * File:   PortableArcade.h
 * Author: callwyat
 *
 * Created on August 6, 2022, 5:27 PM
 */

#ifndef PORTABLEARCADE_H
#define	PORTABLEARCADE_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef union UserInputs
    {
      struct
      {
        bool joyLeft:1;
        bool joyRight:1;
        bool joyUp:1;
        bool joyDown:1;
        bool button1:1;
        bool button2:1;
        bool button3:1;
        bool button4:1;
      };
      struct 
      {
        unsigned char allBits;
      };
    };


    void UpdateScoreBoard(uint16_t value);
    
    void UpdateBonusBoard(uint16_t value);
    
    
#ifdef	__cplusplus
}
#endif

#endif	/* PORTABLEARCADE_H */

