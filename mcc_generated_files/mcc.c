/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.c

  @Summary:
    This is the mcc.c file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.95-b-SNAPSHOT
        Device            :  PIC24FJ128GB406
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36
        MPLAB             :  MPLAB X v5.10
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


// Configuration bits: selected in the GUI

// FSEC
#pragma config BWRP = OFF    //Boot Segment Write Protect->Boot segment may be written
#pragma config BSS = OFF    //Boot segment Protect->No Protection (other than BWRP)
#pragma config BSEN = OFF    //Boot Segment Control bit->No Boot Segment
#pragma config GWRP = OFF    //General Segment Write Protect->Writes to program memory are allowed
#pragma config GSS = OFF    //General Segment Code Protect->Code protection is disabled
#pragma config CWRP = OFF    //Configuration Segment Program Write Protection bit->Configuration Segment may be written
#pragma config CSS = DIS    //Configuration Segment Code Protection Level bits->No Protection (other than CWRP)
#pragma config AIVTDIS = DISABLE    //Alternate Interrupt Vector Table Disable bit->Disable AIVT

// FBSLIM
#pragma config BSLIM = 8191    //Boot Segment Code Flash Page Address Limit bits->8191

// FOSCSEL
#pragma config FNOSC = PRIPLL    //Oscillator Select->Primary Oscillator with PLL module (XTPLL, HSPLL, ECPLL)
#pragma config PLLMODE = PLL96DIV4    //Frequency Multiplier Select Bits->96 MHz PLL (16 MHz input)
#pragma config IESO = OFF    //Internal External Switchover->Start up with user-selected oscillator source

// FOSC
#pragma config POSCMOD = HS    //Primary Oscillator Select->HS Oscillator mode selected
#pragma config OSCIOFCN = ON    //OSCO Pin Configuration->OSCO/CLKO/RC15 functions as port I/O (RC15)
#pragma config SOSCSEL = OFF    //SOSC Power Selection Configuration bits->Digital (SCLKI) mode
#pragma config PLLSS = PLL_PRI    //PLL Secondary Selection Configuration bit->PLL is fed by the Primary oscillator
#pragma config IOL1WAY = ON    //IOLOCK One-Way Set Enable->Once set the IOLOCK bit cannot be cleared
#pragma config FCKSM = CSECMD    //Clock Switching and Monitor Selection->Clock switching is enabled, Fail-Safe Clock Monitor is disabled

// FWDT
#pragma config WDTPS = PS32768    //Watchdog Timer Postscaler->1:32768
#pragma config FWPSA = PR128    //WDT Prescaler->Prescaler ratio of 1:128
#pragma config FWDTEN = OFF    //Watchdog Timer Enable->Watchdog Timer is disabled
#pragma config WINDIS = OFF    //Windowed Watchdog Timer Disable bit->Standard Watchdog Timer enabled (Windowed-mode is disabled)
#pragma config WDTWIN = PS25_0    //Watchdog Window Select bits->Watch Dog Timer Window Width is 25 percent
#pragma config WDTCMX = WDTCLK    //WDT Clock Source Select bits->WDT clock source is determined by the WDTCLK Configuration bits
#pragma config WDTCLK = LPRC    //WDT Clock Source Select bits->WDT uses LPRC

// FPOR
#pragma config BOREN = ON    //Brown-out Reset Enable bits->Brown-out Reset Enable
#pragma config LPCFG = OFF    //Low power regulator control->Disabled

// FICD
#pragma config ICS = PGx2    //Emulator Pin Placement Select bits->Emulator functions are shared with PGEC2/PGED2
#pragma config JTAGEN = OFF    //JTAG Port Enable->JTAG port is disabled
#pragma config BTSWP = OFF    //BOOTSWP Instruction Enable bit->BOOTSWP instruction is disabled

// FDS
#pragma config DSWDTPS = DSWDTPS1F    //Deep Sleep Watchdog Timer Postscale Select bits->1:68,719,476,736 (25.7 days)
#pragma config DSWDTOSC = LPRC    //DSWDT Reference Clock Select bit->DSWDT uses Low Power RC Oscillator (LPRC)
#pragma config DSBOREN = ON    //Deep Sleep Zero-Power BOR Enable bit->Deep Sleep BOR enabled in Deep Sleep
#pragma config DSWDTEN = ON    //Deep Sleep Watchdog Timer Enable bit->DSWDT enabled
#pragma config DSSWEN = ON    //Deep Sleep Software Control Select Bit->Deep Sleep enabled and controlled by the DSEN bit

// FDEVOPT1
#pragma config ALTCMPI = DISABLE    //Alternate Comparator Input Enable bit->C1INC, C2INC, and C3INC are on their standard pin locations
#pragma config TMPRPIN = OFF    //Tamper Pin Enable bit->TMPRN pin function is disabled
#pragma config TMPRWIPE = OFF    //RAM Based Entryption Key Wipe Enable bit->Cryptographic Engine Key RAM is not erased onTMPR pin events
#pragma config ALTVREF = ALTVREFEN    //Alternate VREF location Enable->VREF is on an alternate pin (VREF+ on RB0 and VREF- on RB1)

// FBOOT
#pragma config BTMODE = SINGLE    //Boot Mode Select bits->Device is in Single Boot (legacy) mode

#include "mcc.h"
#include "clock.h"


void OSCILLATOR_Initialize(void)
{
    CLOCK_Initialize();
}

/**
 End of File
*/