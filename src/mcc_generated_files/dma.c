
#include <stdint.h>
#include "xc.h"
#include "clock.h"

void DMA_Initialize(void)
{
    DMAL = 0x0000; //Setup addressable space
    DMAH = 0x7FFF;
    DMACON = 0x8000; //Enable the DMA Modules

    //DMA5      This channel is used for USB Transfers
    DMAINT5 = 0x0000; //No Trigger (ALU commands DMA to start)
    DMACH5 = 0x0059;  //Enable Module, 8-Bit, Continuous
}
