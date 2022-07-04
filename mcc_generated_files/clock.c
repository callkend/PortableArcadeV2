/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    clock.c

  @Summary:
    This is the clock.c file generated using PIC24 / dsPIC33 / PIC32MM MCUs

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

#include <stdint.h>
#include "xc.h"
#include "clock.h"

void CLOCK_Initialize(void)
{
    //Temporarily switch to FRC (without PLL), so we can safely change the PLL settings,
    //in case we had previously been already running from the PLL.
    __builtin_write_OSCCONH((uint8_t) (OSCCONH & 0xF8));
    if(OSCCONbits.COSC != OSCCONbits.NOSC)
    {
        __builtin_write_OSCCONL((uint8_t) (OSCCONL & 0xFF));            //Initiate clock switching operation.
        while(OSCCONbits.OSWEN != 0);    //Wait for switching complete.
        while (OSCCONbits.LOCK != 1);    //Wait till PLL is locked
    }
    // CPDIV 1:1; PLLEN disabled; RCDIV FRC/2; DOZE 1:8; DOZEN disabled; ROI disabled; 
    CLKDIV = 0x3100;
    // STOR disabled; STORPOL Interrupt when STOR is 1; STSIDL disabled; STLPOL Interrupt when STLOCK is 1; STLOCK disabled; STSRC SOSC; STEN disabled; TUN Center frequency; 
    OSCTUN = 0x00;
    // ROEN enabled; ROSWEN disabled; ROSEL REFCLKI; ROOUT enabled; ROSIDL disabled; ROSLP disabled; 
    REFOCONL = 0x9008;
    // RODIV 0; 
    REFOCONH = 0x00;
    // ADC1MD enabled; T3MD enabled; T4MD enabled; T1MD enabled; U2MD enabled; T2MD enabled; U1MD enabled; SPI2MD enabled; SPI1MD enabled; T5MD enabled; I2C1MD enabled; 
    PMD1 = 0x00;
    // OC5MD enabled; IC6MD enabled; OC6MD enabled; IC5MD enabled; IC4MD enabled; IC3MD enabled; OC1MD enabled; IC2MD enabled; OC2MD enabled; IC1MD enabled; OC3MD enabled; OC4MD enabled; 
    PMD2 = 0x00;
    // I2C3MD enabled; PMPMD enabled; U3MD enabled; CMPMD enabled; CRCMD enabled; I2C2MD enabled; DAC1MD enabled; 
    PMD3 = 0x00;
    // U4MD enabled; USB1MD enabled; CTMUMD enabled; REFOMD enabled; HLVDMD enabled; 
    PMD4 = 0x00;
    // CCP2MD enabled; CCP1MD enabled; CCP4MD enabled; CCP3MD enabled; CCP7MD enabled; CCP5MD enabled; CCP6MD enabled; 
    PMD5 = 0x00;
    // SPI4MD enabled; SPI3MD enabled; LCDMD enabled; 
    PMD6 = 0x00;
    // DMA1MD enabled; DMA0MD enabled; 
    PMD7 = 0x00;
    // U5MD enabled; CLC3MD enabled; CLC4MD enabled; CRYMD enabled; CLC1MD enabled; CLC2MD enabled; U6MD enabled; 
    PMD8 = 0x00;
    // CF no clock failure; NOSC PRIPLL; SOSCEN disabled; POSCEN disabled; CLKLOCK unlocked; OSWEN Switch is Complete; IOLOCK not-active; 
    __builtin_write_OSCCONH((uint8_t) (0x03));
    __builtin_write_OSCCONL((uint8_t) (0x01));
    // Wait for Clock switch to occur
    while (OSCCONbits.OSWEN != 0);
    while (OSCCONbits.LOCK != 1);
}
