#include <msp430.h>

/**
 * @file msp430f5529_button_interrupt.c
 * @author John McAvoy
 * @date 14 Oct 2018
 */
#define LED BIT0 // P1.0
#define BTN BIT1 // P2.1

void stop_wtd(){
    WDTCTL = WDTPW | WDTHOLD;  // stop watchdog timer
}

void setup_led(){
    // set LED output
    P1SEL &= ~(LED);
    P1DIR |= LED;
}

void setup_button_interrupt(){
    P2IE |= BTN; // set BTN as interrupt
}

int main(void) {
    stop_wtd();

    setup_led();

    setup_button_interrupt();

    // infinite loop
    while(1) {
        __bis_SR_register(LPM4_bits + GIE); // Enter LPM4 w/interrupt
    }

    return 0;
}

// Port 2 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT2_VECTOR))) Port_2 (void)
#else
#error Compiler not supported!
#endif
{
  P1OUT ^= LED; // LED = toggle
  P2IFG &= ~(BTN); // BTN IFG cleared
}
