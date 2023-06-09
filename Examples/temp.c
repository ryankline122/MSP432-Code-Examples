/*
 * This code demonstrates how to interface with a 16x4 LCD display on the MSP432P401R.
 *
 * The LCD screen will display the current value of "count", which can be incremented/decremented
 * by using the on board switches. Refer to digitalIO.c for using switches.
 *
 * Wiring is as follows:
 *      PORT4 PIN 4.0 – RS
 *      PORT4 PIN 4.1 – RW
 *      PORT4 PIN 4.2 – EN
 *      PORT4 PIN 4.4 – D4
 *      PORT4 PIN 4.5 – D5
 *      PORT4 PIN 4.6 – D6
 *      PORT4 PIN 4.6 – D7
 *      VDD & Anode (A) to 5V
 *      VSS & Cathode (K) to GND
 *      V0 to the output of a 10k Potentiometer
 */

#include "msp432.h"
#include "msp.h"
#include <stdio.h>

#define RS 1     /* P4.0 mask */
#define RW 2     /* P4.1 mask */
#define EN 4     /* P4.2 mask */

// LCD Commands
#define CLEAR 0x01
#define SETUP 0x28
#define INCREMENT_CURSOR 0x06
#define ACTIVATE 0x0F
#define HOME 0x80

void delayMs(int n);
void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
void lcd_print(char *string);

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
        NVIC->ISER[0] |= 1 << ((ADC14_IRQn)&31); // enable nvic

        // enable global interrupts
        __enable_irq();

        while (1){
            delayMs(500);                       // delay
            ADC14->CTL0 |= ADC14_CTL0_ENC | ADC14_CTL0_SC; // start conversion and enable conversion
        }
}

void ADC14_IRQHandler(void) {
    uint32_t adcValue = ADC14->MEM[0];
    float voltage = adcValue * (3.3 / 4096.0);
    float temperatureCelsius = ((voltage - 0.5) * 100) - 216.27;
    float tempF = (temperatureCelsius * (9/5)) + 32;
    char output_str[32];

    // Write temperature to LCD
    sprintf(output_str, "Current Temp:   %.2f F", tempF);
    lcd_print(output_str);

}

///*
// * Clears display and sets cursor to home position before
// * writing new data to the display
// */
//void lcd_print(char *string) {
//    LCD_command(CLEAR);
//    LCD_command(HOME);
//
//    int i = 0;
//    while(string[i] != '\0') {
//        LCD_data(string[i]);
//        i++;
//    }
//}
//
///*
// * Setup output pins and configures LCD behaviors.
// */
//void LCD_init(void) {
//    P4->DIR = 0xFF;             /* make P4 pins output for data and controls */
//    delayMs(30);                /* initialization sequence */
//    LCD_nibble_write(0x30, 0);
//    delayMs(10);
//    LCD_nibble_write(0x30, 0);
//    delayMs(1);
//    LCD_nibble_write(0x30, 0);
//    delayMs(1);
//    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
//    delayMs(1);
//
//    LCD_command(SETUP);
//    LCD_command(INCREMENT_CURSOR);
//    LCD_command(CLEAR);
//    LCD_command(ACTIVATE);
//}
//
///*
// * Sends a 4-bit nibble (half of a byte) to the LCD display by:
// *
// *  - Setting the appropriate pins on Port 4 to control the RS, RW, EN,
// *    and data signals
// *  - Pulsing the EN pin to trigger the LCD
// *  - Introducing a short delay for the LCD to process the command or data
// *  - Clearing the pins to prepare for the next operation.
// */
//void LCD_nibble_write(unsigned char data, unsigned char control) {
//    data &= 0xF0;           /* clear lower nibble for control */
//    control &= 0x0F;        /* clear upper nibble for data */
//    P4->OUT = data | control;       /* RS = 0, R/W = 0 */
//    P4->OUT = data | control | EN;  /* pulse E */
//    delayMs(0);
//    P4->OUT = data;                 /* clear E */
//    P4->OUT = 0;
//}
//
///*
// * Handles commands for the LCD screen.
// *
// * Commands:
// *      - 0x28: Sets LCD to 4-bit data mode, 2 line display, 5x7 font
// *      - 0x06: Sets the entry mode to increment cursor position after each character
// *      - 0x01: Clears the display and moves cursor to home position
// *      - 0x0F: Turns on display and activates cursor
// */
//void LCD_command(unsigned char command) {
//    LCD_nibble_write(command & 0xF0, 0);    /* upper nibble first */
//    LCD_nibble_write(command << 4, 0);      /* then lower nibble */
//
//    if (command < 4)
//        delayMs(4);         /* commands 1 and 2 need up to 1.64ms */
//    else
//        delayMs(1);         /* all others 40 us */
//}
//
///*
// * Writes data to LCD
// */
//void LCD_data(unsigned char data) {
//    LCD_nibble_write(data & 0xF0, RS);    /* upper nibble first */
//    LCD_nibble_write(data << 4, RS);      /* then lower nibble  */
//
//    delayMs(1);
//}
//
///*
// * Delay milliseconds when system clock is at 3 MHz
// */
//void delayMs(int n) {
//    int i, j;
//
//    for (j = 0; j < n; j++)
//        for (i = 750; i > 0; i--);      /* Delay */
//}
