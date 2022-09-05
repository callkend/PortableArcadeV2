
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#include "SDCard.h"
#include "../mcc_generated_files/spi3.h"

extern bool SDCardPresent(void);

extern void SetSDCardCS(bool value);

volatile uint16_t currentSPIHandle = 0;

uint16_t SDExchangeCommand(SDCardCommand_t command, uint8_t *responseBuffer, uint16_t responseLength)
{        
    SetSDCardCS(true);
    
    uint8_t *outputPnt = responseBuffer;

    // Write the whole command
    SPI3BUFL = command.Command;    
    SPI3BUFL = command.Arg3;
    SPI3BUFL = command.Arg2;
    SPI3BUFL = command.Arg1;
    SPI3BUFL = command.Arg0;    
    SPI3BUFL = command.CRC;
    
    uint8_t temp;

    // Wait for the command to be done sending
    while( SPI3STATLbits.SRMT == false )
    {
        // Work on flushing the buffer;
        if (SPI3STATLbits.SPIRBE == false)
            temp = SPI3BUFL;
    }
    
    // while( SPI3STATLbits.SPITBE == false );
    
    // Make sure it is empty
    while ( SPI3STATLbits.SPIRBE == false) {
        temp = SPI3BUFL;
    }
    
    // Wait for the SDCard to start being responsive
    uint8_t attemptCount = 100;
    do
    {
        if (--attemptCount == 0)
        {
            SetSDCardCS(false);
            return 0;
        }
        
        SPI3BUFL = 0xFF;
        // Wait for transfer to complete
        while ( SPI3STATLbits.SPIRBE == true);
        
        *outputPnt = SPI3BUFL;
    } while (*outputPnt == 0xFF);
    
    ++outputPnt;
    
    // Capture the response
    while (--responseLength)
    {
        SPI3BUFL = 0xFF;
        // Wait for transfer to complete
        while( SPI3STATLbits.SPITBE == false );
        *outputPnt++ = SPI3BUFL;
    }
    
    while( SPI3STATLbits.SRMT == false );
    
    SetSDCardCS(false);
    return (uint16_t)(outputPnt - responseBuffer);
}

uint8_t SDCardInit(void)
{
    if (SDCardPresent())
    {
        // Set Baud Rate to less then 400k
        SPI3_SetBaudRate(400000);
        
        // 80 Clocks while not asserted
        SetSDCardCS(false);
        
        for (uint8_t i = 0; i < 10; ++i)
        {
            SPI3BUFL = 0xFF;
        }
        
        while( SPI3STATLbits.SRMT == false );
        
        uint8_t responseBuffer[6];
        
        {
            // COM_0 -> Soft Reset
            SDCardCommand_t com0Command = SDCommand(0x40, 0x00, 0x00, 0x00, 0x00, 0x95);        
            SDExchangeCommand(com0Command, responseBuffer, 1);
        }
        {
            // COM_8 -> Wait for the card to be ready
            SDCardCommand_t com8Command = SDCommand(0x48, 0x00, 0x00, 0x01, 0xAA, 0x86);
            responseBuffer[0] = 0x00;
            for (uint8_t attempt = 0; responseBuffer[0] != 0x01; ++attempt)
            {
                if (attempt > 100)
                {
                    return 0x02;
                }

                SDExchangeCommand(com8Command, responseBuffer, 1);
            }
        }
        // Check that the card can run at 3V3.
        {
            // COM_58 -> Check the supported voltage levels of the card
            SDCardCommand_t com58Command = SDCommand(0x7A, 0x00, 0x00, 0x00, 0x00, 0x95);
            SDExchangeCommand(com58Command, responseBuffer, 4);
            
            // Check if a 3V3 is valid for this SD Card
            if ((responseBuffer[2] & 0x10) == 0)
            {
                return 0x03;
            }
        }
        {
            // COM_55 -> Prepare for the app command
            SDCardCommand_t com55Command = SDCommand(0x77, 0x00, 0x00, 0x00, 0x00, 0x7A);
            // COM_41 -> Leave Idle Mode
            SDCardCommand_t com41Command = SDCommand(0x69, 0x40, 0x00, 0x00, 0x00, 0x95);

            for (uint8_t attempt = 0; responseBuffer[0] != 0x01; ++attempt)
            {
                if (attempt > 100)
                {
                    return 0x04;
                }

                SDExchangeCommand(com55Command, responseBuffer, 1);           
                SDExchangeCommand(com41Command, responseBuffer, 1);
            }
        }
        
        return 0x00;
    }
    
    SetSDCardCS(false);
    return 0x01;
}

void SDReadDataBlock(uint32_t address, uint8_t *buffer)
{
    
}

