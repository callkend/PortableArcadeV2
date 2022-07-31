
#include <xc.h>

void EPMP_Initialize(void)
{
    // Address on separate pins, Master Mode, CS2 is CS2, Smart Address, Idle Z, No Interrupts
    PMCON1 = 0x0310;
    
    // Clear Status bits
    PMCON2 = 0x0000;
    
    // Enable WS, RS, Min Latch Strobes and Hold Strobes
    PMCON3 = 0xC000;
    
    // Enable Address lines 0-4
    PMCON4 = 0x001F;
    
    // Disable all the chip selects
    PMCS1CFbits.CSDIS = 1;
    PMCS2CFbits.CSDIS = 1;
        
    // Enable the port
    PMCON1bits.PMPEN = 1;
    
}

