
#include <xc.h>

// Magically gets called before main
void __init_PMP(void)
{
    // Reset
    LATFbits.LATF5 = 0;
    TRISFbits.TRISF5 = 0;
    ANSFbits.ANSF5 = 0;
    
    // DataPort
    TRISE |= 0x00FF;
    ANSE &= 0xFF00;
    
    // Address on separate pins, Master Mode, CS2 is CS2, Smart Address, Idle Z, No Interrupts
    PMCON1 = 0x0310;
    
    // Clear Status bits
    PMCON2 = 0x0000;
    
    // Enable WS, RS, Min Latch Strobes and Hold Strobes
    PMCON3 = 0xC000;
    
    PMCON3bits.AWAITE = 1;

    // Enable Address lines 0-4
    PMCON4 = 0x001F;
    
    // Enable CS1 Bank, WR and RD active low, 8 bit bus
    PMCS1CF = 0x0000;

    // Use this setting to extend the read and write strobe
    PMCS1MDbits.DWAITM = 1;
    
    // Don't use the CS2 bank
    PMCS2CFbits.CSDIS = 1;
        
    // Enable the port
    PMCON1bits.PMPEN = 1;  
    
    LATFbits.LATF5 = 1;
}

