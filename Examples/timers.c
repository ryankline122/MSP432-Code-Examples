/*
 * This code demonstrates how to set up a timer with an interrupt on the MSP432P401R.
 *
 * The timer counts clock ticks from a clock source, and when it overflows, an interrupt
 * is triggered. The ISR function toggles an LED connected to the micro-controller,
 * demonstrating how timers and interrupts can be used for tasks like controlling the
 * blinking rate of the LED.
 *
 */

#include "msp.h"

void TA0_N_IRQHandler(void);

void timers(void)
{

    // Setup ports

    // Set P1.0 as GPIO
    P1->SEL0 &= ~BIT0;
    P1->SEL1 &= ~BIT0;

	// LED
    P1->DIR |= BIT0;   				// Set P1.0 as output
	P1->OUT &= ~BIT0;				// Clear P1.0 initially

    // Setup timer
	TA0CTL |= TACLR;                // Resets timer
	TA0CTL |= TASSEL__ACLK;         // Configures timer A0 to use the ACLK (auxillary clock) as its clock source
	TA0CTL |= MC__CONTINUOUS;       // Continuous mode (counts up to max value, then rolls over to 0)

    // Setup timer overflow
    TA0CTL |= TAIE;                 // Enables interrupt on timer overflow
    __enable_irq();                 // Enables global interrupts
    NVIC_EnableIRQ(TA0_N_IRQn);     // Registers the interrupt function with the Nested Vectored Interrupt Controller (NVIC)
    TA0CTL &= ~TAIFG;               // Clear IRQ Flag


    while(1){}
}


void TA0_N_IRQHandler(void)
{
    P1->OUT ^= BIT0;                // Toggle LED

    int i = 0;
    for(i; i<20000; i++){}          // Delay

    TA0CTL &= ~TAIFG;               // Clear IRQ flag
}
