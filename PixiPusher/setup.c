
#include <xc.h>

#define Button PORTDbits.RD6

void Setup()
{
    
    // Turn on the LED
    ANSFbits.ANSF0 = 0;
    TRISFbits.TRISF0 = 0;
    
    PORTFbits.RF0 = 1;
    
    // Enable the button
    ANSDbits.ANSD6 = 0;
    TRISDbits.TRISD6 = 1;
    // Enable Week-Pull-Up
    IOCPDDbits.CNPDD6 = 1;
    
    // Enable Falling Edge Interrupts on the button
    IOCPDbits.IOCPD6 = 1;
    
    // Globally enable Falling Edge Interrupts
    PADCONbits.IOCON = 1;
    IEC1bits.CNIE = 1;
}
