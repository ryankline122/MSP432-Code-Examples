#include "msp.h"
#include "stdio.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	P1DIR = 0x01; // Set Pin 0 as output

	while(1){
	    P1DIR ^= 0x01;
	    unsigned int i = 0;

	}

}
