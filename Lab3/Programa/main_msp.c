// Mathias Lambert V. (c) - Lab3 SEP 2018-2
// Task 1

#include <msp430.h>
#include <stdint.h>
#include "bits.h"

unsigned char val = (1 << 4);
int aux = 0;
int mode = 0;
int cuenta = 0;
int no_pause = 1;

int main(void) {
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer, it runs by default.

    P1DIR |= BIT0;        // Port1 direction register, Pin0 as output
    P4DIR |= BIT7;        // Port7 direction register, Pin7 as output

    P6DIR |= BIT5;        // Salida a Led P6.5
    P3DIR |= BIT4;        // Salida a Led P3.4
    P3DIR |= BIT3;        // Salida a Led P3.3
    P1DIR |= BIT6;        // Salida a Led P1.6
    P6DIR |= BIT6;        // Salida a Led P6.6

    P1REN |= BIT1;	      // Resistencia activa en P1.1 (S2)
    P1OUT |= BIT1;	      // Configurada como pull-up

    P2REN |= BIT1;	      // Resistencia activa en P2.1 (S1)
    P2OUT |= BIT1;	      // Configurada como pull-up


    P2IE |= BIT1;		  // Habilita la interrupción de P2.1
    P2IES |= BIT1;	      // Interrupción por flanco de bajada
    P2IFG &= ~(BIT1);	  // Baja flag de interrupción de P2.1

    P1IE |= BIT1;		  // Habilita la interrupción de P1.1
    P1IES |= BIT1;	      // Interrupción por flanco de bajada
    P1IFG &= ~(BIT1);	  // Baja flag de interrupción de P1.1

    // Configuración TIMER_A:
    // TimerA1, ACLK/1,modo up, reinicia TACLR
    TA1CTL = TASSEL_1 | TACLR | MC_1;
    // ACLK tiene una frecuencia de 32768 Hz
    TA1CCR0 = 4000;
    TA1CCTL0 = CCIE; // Habilita interrupción (bit CCIE)
    // LPM0, habilitación general de interrupciones
    __low_power_mode_0();
    __no_operation();

}

void __attribute__((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR(void) {
    P1OUT ^= BIT0; // Toggle P1.0 (XOR)

    P6OUT &= ~BIT5;
    P3OUT &= ~BIT4;
    P3OUT &= ~BIT3;
    P1OUT &= ~BIT6;
    P6OUT &= ~BIT6;

    P6OUT |= (BIT0 & val) << 5;
    P3OUT |= (BIT1 & val) << 3;
    P3OUT |= (BIT2 & val) << 1;
    P1OUT |= (BIT3 & val) << 3;
    P6OUT |= (BIT4 & val) << 2;

    val = patrones(val, aux, mode);
    if (mode == 1){
        if (val == 1) {
            aux = 1;
        }
        else if (val == BIT4){
            aux = 0;
        }
    }
    if (no_pause == 1){
        cuenta++;
        cuenta = cuenta % 61;
        if (cuenta == 15) {
            val = (1 << 4);
            aux = 0;
            mode = 1;
        }
        else if (cuenta == 40){
            val = 0b00010101;
            mode = 2;
        }
        else if (cuenta == 60){
            val = (1 << 4);
            mode = 0;
        }
    }
}

void __attribute__((interrupt(PORT2_VECTOR))) P2_ISR(void) {
    if (P2IFG & BIT1) {	// comprobar fuente de la interrupción P2.1
        if (!(P2IN & BIT1)){    // Si P2.1 está a 0 (S1 pulsado)
            TA1CCR0 += 4000;
            if (TA1CCR0 > 8000){
                TA1CCR0 = 4000;
            }
        }
        P2IFG &= ~BIT1;	// puesta a cero del flag de interrupción P2.1
    }
}

void __attribute__((interrupt(PORT1_VECTOR))) P1_ISR(void) {
    if (P1IFG & BIT1) {	// comprobar fuente de la interrupción P1.1
        if (!(P1IN & BIT1)){    // Si P1.1 está a 0 (S2 pulsado)
            if (no_pause == 1) {
                no_pause = 0;
                P4OUT ^= BIT7; // Toggle P4.7 (XOR)
            }
            else {
                no_pause = 1;
                P4OUT ^= BIT7; // Toggle P4.7 (XOR)
            }
        }
        P1IFG &= ~BIT1;	// puesta a cero del flag de interrupción P1.1
    }
}
