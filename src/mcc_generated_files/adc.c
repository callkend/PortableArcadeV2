#include <stdint.h>
#include "xc.h"
#include "adc.h"

void ADC_Initialize(void)
{
    //Setup RB12 to be a Analog input
    ANSBbits.ANSB12 = 1;
    TRISBbits.TRISB12 = 1;
    //Enable ADC for manual sampling
    AD1CON1 = 0x8000;
    AD1CON2 = 0x0000;
    AD1CON3 = 0x0000;
    AD1CON4 = 0x0000;
    AD1CON5 = 0x0000;

    //Select the AN12/RB12 Channel that is connected to the photoresisitor.
    AD1CHS = 0x0C0C;
}

void ADC_Start(void)
{
    AD1CON1bits.SAMP = 1; //Start an ADC Sample
}

unsigned char ADC_Read(void)
{
    char ADCResgult;
    while(AD1CON1bits.DONE != 1) //Wait until read ADC conversion is completed
    {
    }
    ADCResult = ADC1BUF0 >> 2;
    return ADCResult; //Return ADC result
}