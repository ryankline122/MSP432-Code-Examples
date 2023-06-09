/*
 * This code demonstrates how to interface with a TMP 36 temperature sensor.
 */

#include "msp432.h"
#include "msp.h"
#include <stdio.h>

void temp(void) {
    LCD_init();

    // Setup Ports
        // Enable ADC Pins
        P5->SEL0 |= BIT0;
        P5->SEL1 |= BIT0;

        // Setup for ADC
        ADC14->CTL0 |= ADC14_CTL0_SHP | ADC14_CTL0_CONSEQ_0 | ADC14_CTL0_ON; //
        ADC14->MCTL[0] = ADC14_MCTLN_INCH_5;                                 // set ADC channel as A5 = P5.0
        ADC14->CTL1 |= ADC14_CTL1_RES__14BIT;                                // set resolution as a 14 bits resolution
        ADC14->IER0 |= ADC14_IER0_IE0;

        // enable NVIC for ADC14
        NVIC->ISER[0] |= 1 << ((ADC14_IRQn)&31);                            // enable nvic

        // enable global interrupts
        __enable_irq();

        while (1){
            delayMs(500);                                                   // delay
            ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC;                  // start conversion and enable conversion
        }
}

void ADC14_IRQHandler(void) {
    uint32_t adcValue = ADC14->MEM[0];
    float voltage = adcValue * (3.3 / 4096.0);
    float temperatureCelsius = ((voltage - 0.5) * 100);
    float tempF = (temperatureCelsius * (9/5)) + 32;
    char output_str[32];

    // Write temperature to LCD
    sprintf(output_str, "Current Temp:   %.2f F", tempF);
    lcd_print(output_str);

}

