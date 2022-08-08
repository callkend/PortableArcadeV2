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

    typedef union UserInput_t
    {
      struct
      {
        bool JoyLeft:1;
        bool JoyRight:1;
        bool JoyUp:1;
        bool JoyDown:1;
        bool Button1:1;
        bool Button2:1;
        bool Button3:1;
        bool Button4:1;
      };
      struct 
      {
        uint8_t AllBits;
      };
    } UserInput_t;


    void UpdateScoreBoard(uint16_t value);
    
    void UpdateBonusBoard(uint16_t value);
    
    UserInput_t ReadUserInputs(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PORTABLEARCADE_H */

