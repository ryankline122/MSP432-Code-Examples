#include "msp.h"

void PORT1_IRQHandler(void);

void interrupts(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	// Setup Ports

    // Set P1.0, P1.1 as GPIO
    P1->SEL0 &= ~(BIT0 | BIT1);
    P1->SEL1 &= ~(BIT0 | BIT1);

	// LED
    P1->DIR |= BIT0;   				// Set P1.0 as output
	P1->OUT &= ~BIT0;				// Clear P1.0 initially

	// Switch
    P1->DIR &= ~BIT1;   			// Set P1.1 as input 
    P1->REN |= BIT1;    			// Enables resistor
    P1->OUT |= BIT1;    			// Sets resistor to pull up
    P1->IE |= BIT1;                 // Interrupt enabler
    P1->IES |= BIT1;                // Sets sensitivity H-to-L

    //-- setup IRQ
    NVIC_EnableIRQ(PORT1_IRQn);
    __enable_irq();

    while(1){}

}

void PORT1_IRQHandler(void){
    P1->OUT ^= BIT0;
    int i = 0;
    for(i; i<10000; i++){}
    P1->IFG &= ~BIT1;
}
