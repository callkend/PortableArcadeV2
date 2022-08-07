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
    
//    typedef union ScoreBoard {
//        struct {
//            char Cells[4];
//        };
//                
//        struct {
//            char Cell0;
//            char Cell1;          
//            char Cell2;          
//            char Cell3;          
//        };
//    }; 
    
    __eds__ uint8_t __attribute__((noload, address(0x008000))) EXTNIN1 __attribute__((space(eds)));
    __eds__ uint8_t __attribute__((noload, address(0x008002))) EXTNIN2 __attribute__((space(eds)));

    __eds__ uint8_t __attribute__((noload, address(0x008004))) EXT_RES2 __attribute__((space(eds)));
    
    __eds__ uint8_t __attribute__((noload, address(0x008006))) EXTNOUT1 __attribute__((space(eds)));
    
    __eds__ uint8_t __attribute__((noload, address(0x008008))) EXT_RES4 __attribute__((space(eds)));    
    __eds__ uint8_t __attribute__((noload, address(0x00800A))) EXT_RES5 __attribute__((space(eds)));
    __eds__ uint8_t __attribute__((noload, address(0x00800C))) EXT_RES6 __attribute__((space(eds)));
    __eds__ uint8_t __attribute__((noload, address(0x00800E))) EXT_RES7 __attribute__((space(eds)));

    __eds__ uint16_t __attribute__((noload, address(0x008010))) SCOREDISPLAY[4] __attribute__((space(eds)));
    
    __eds__ uint16_t __attribute__((noload, address(0x008018))) BONUSDISPLAY[4] __attribute__((space(eds)));
    

#ifdef	__cplusplus
}
#endif

#endif	/* EPMP_H */

