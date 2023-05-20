#include "msp.h"

void blinky()
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

/*==== Blinky "Hello World" ====*/
// LED1 (red) should blink indefinitely


	P1->SEL0 &= ~BIT0; 								// Set P1.0 SEL for GPIO 
	P1->SEL1 &= ~BIT0; 							

	P1->DIR |= BIT0;								// Set P1.0 as output
	P1->OUT |= BIT0;								// Set P1.0 HIGH

	while(1){
	    P1DIR ^= BIT0;								// Toggle LED

	    unsigned int i = 0;
		for(i; i<0xFFFF; i++){}
	}

}