// Mathías Lambert V. (c) - Lab10 SEP 2018-2

#include <msp430.h>


void init_lab(void) {
    P1DIR |= BIT0;               // P1.0 salida
    P1OUT |= BIT0;               // Enciende LED

    P4DIR |= BIT7;               // P4.7 salida
    P4OUT &= ~BIT7;               // Apaga LED

    // Configuración TIMER_A:
    // TimerA1, ACLK/1,modo up, reinicia TACLR
    TA1CTL = TASSEL_1 | TACLR | MC_1;
    // ACLK tiene una frecuencia de 32768 Hz
    // Carga cuenta en TA1CCR0 0.5seg TA1CCR=(0,5*32768)-1
    TA1CCR0 = 16383;
    TA1CCTL0 = CCIE; // Habilita interrupción (bit CCIE)

    __bis_SR_register(GIE);
}

void __attribute__((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR(void) {
    P1OUT ^= BIT0;
    P4OUT ^= BIT7;
}
