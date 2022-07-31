
#include <xc.h>

void __init_PMP(void)
{
    // Reset
    LATFbits.LATF5 = 0;
    TRISFbits.TRISF5 = 0;
    ANSFbits.ANSF5 = 0;
    
    // Address on separate pins, Master Mode, CS2 is CS2, Smart Address, Idle Z, No Interrupts
    PMCON1 = 0x0310;
    
    // Clear Status bits
    PMCON2 = 0x0000;
    
    // Enable WS, RS, Min Latch Strobes and Hold Strobes
    PMCON3 = 0xC000;
    
    // Enable Address lines 0-4
    PMCON4 = 0x001F;
    
    // Disable all the chip selects
    PMCS1CFbits.CSDIS = 0;
    PMCS2CFbits.CSDIS = 1;
        
    // Enable the port
    PMCON1bits.PMPEN = 1;  
    
    LATFbits.LATF5 = 1;
}

