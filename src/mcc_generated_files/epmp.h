/* 
 * File:   epmp.h
 * Author: callwyat
 *
 * Created on July 30, 2022, 9:40 PM
 */

#ifndef EPMP_H
#define	EPMP_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    typedef union ScoreBoard {
        struct {
            char Cells[4];
        };
                
        struct {
            char Cell0;
            char Cell1;          
            char Cell2;          
            char Cell3;          
        };
    };
    
    // __eds__ ScoreBoard __attribute__((eds, address(0x008000))) SCOREDISPLAY;
    
    // __eds__ ScoreBoard __attribute__((eds, address(0x008008))) BONUSDISPLAY;    
    
    __eds__ unsigned char __attribute__((eds, address(0x008010))) EXTNIN1;

    __eds__ unsigned char __attribute__((eds, address(0x008012))) EXTNIN2;
    
    __eds__ unsigned char __attribute__((eds, address(0x00801A))) EXTNOUT1;


    void EPMP_Initialize(void);


#ifdef	__cplusplus
}
#endif

#endif	/* EPMP_H */

