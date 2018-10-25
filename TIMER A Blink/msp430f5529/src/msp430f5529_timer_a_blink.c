#include <msp430.h>

/**
 * @file msp430f5529_timer_a_blink.c
 * @author John McAvoy
 * @date 14 Oct 2018
 */
#define LED1 BIT0 // P1.0
#define LED2 BIT7 // P4.7

void stop_wdt() {

    WDTCTL = WDTPW | WDTHOLD;  // stop watchdog timer
}

void setup_leds() {

    // set LEDs to output
    P1DIR |= LED1;
    P4DIR |= LED2;

    // set LEDs low
    P1OUT &= ~LED1;
    P4OUT &= ~LED2;
}

void setup_timers() {

    // SETUP TIMER0
    // set Timer0 ccr
    TA0CCR0 = 50000;
    // A clock / 3, upmode
    TA0CTL = TASSEL_2 + ID_3 + MC_1;

    // enable CC interrupt
    TA0CCTL0 |= CCIE;

    // SETUP TIMER1
    // set Timer1 ccr
    TA1CCR0 = 10000;
    // A clock / 3, upmode
    TA1CTL = TASSEL_2 + ID_3 + MC_1;

    // enable CC interrupt
    TA1CCTL0 |= CCIE;
}

int main(void) {

    stop_wdt();

    setup_leds();

    setup_timers();

    _BIS_SR(LPM0_bits + GIE);

    while(1) {}
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer0_A0 (void)
#else
#error Compiler not supported!
#endif
{
    P1OUT ^= LED1; // toggle LED1
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer1_A0 (void)
#else
#error Compiler not supported!
#endif
{
    P4OUT ^= LED2; // toggle LED2
}
