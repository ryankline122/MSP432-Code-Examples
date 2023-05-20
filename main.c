#include "msp.h"
#include <msp432.h>
#include "stdio.h"

void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

/*==== Blinky "Hello World" ====*/
// LED1 (red) should blink indefinitely


	// P1->SEL0 &= ~BIT0; 								// Set P1.0 SEL for GPIO 
	// P1->SEL1 &= ~BIT0; 							

	// P1->DIR |= BIT0;								// Set P1.0 as output
	// P1->OUT |= BIT0;								// Set P1.0 HIGH

	// while(1){
	//     P1DIR ^= BIT0;								// Toggle LED

	//     unsigned int i = 0;
	// 	for(i; i<0xFFFF; i++){}
	// }


/*==== Using the Switches to control LEDs ====*/
// SW1 should toggle LED1 (red)
// SW2 should toggle LED2 (blue)


// Setup Ports
    // Set P1.0, P1.1, P1.4, and P2.2 as GPIO
    P1->SEL0 &= ~(BIT0 | BIT1 | BIT4);
    P1->SEL1 &= ~(BIT0 | BIT1 | BIT4);
    P2->SEL0 &= ~BIT2;				 
    P2->SEL1 &= ~BIT2;

    P1->DIR |= BIT0;   				// Set P1.0 as output
    P2->DIR |= BIT2;   				// Set P2.2 as output
    P1->DIR &= ~(BIT1 | BIT4);   	// Set P1.1 and P1.4 as input 

    P1->REN |= (BIT1 | BIT4);    	// Enables resistor
    P1->OUT |= (BIT1 | BIT4);    	// Sets resistor to pull up

    int SW1;
    int SW2;
    while(1){
        SW1 = P1->IN;   // Read Switch Port 1
        SW2 = P1->IN;   // Read Switch Port 2

        SW1 &= BIT1;    // Clear all bits except SW1
        SW2 &= BIT4;    // Clear all bits except SW2

        if (SW1 == 0){
            P1OUT |= BIT0;      // Turn on LED1
        }else{
            P1OUT &= ~BIT0;     // Turn off LED1
        }

        if (SW2 == 0){
            P2OUT |= BIT2;      // Turn on LED2
        }else{
            P2OUT &= ~BIT2;     // Turn off LED2
        }
    }

}
