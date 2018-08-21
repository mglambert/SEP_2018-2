// Mathias Lambert V. (c) - Lab2 SEP 2018-2
#include <msp430.h>
#include <stdint.h>


int main(void){
    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer, it runs by default.

    P1DIR |= 0x01;        // Port1 direction register, Pin0 as output
    P4DIR |= BIT7;        // Port7 direction register, Pin7 as output

    P1REN |= BIT1;	      // Resistencia activa en P1.1 (S2)
    P1OUT |= BIT1;	      // Configurada como pull-up

    P2REN |= BIT1;	      // Resistencia activa en P2.1 (S1)
    P2OUT |= BIT1;	      // Configurada como pull-up


    P2IE |= BIT1;		  // habilita la interrupción de P2.1
    P2IES |= BIT1;	      // Interrupción por flanco de bajada
    P2IFG &= ~(BIT1);	  // baja flag de interrupción de P2.1

    // Activar interrupciones
    __enable_interrupt();

    uint32_t k = 2500;

    while (1) // Infinite loop
    {
        P1OUT ^= 0x01; // Toggle P1.0 (XOR)
        P4OUT ^= BIT7; // Toggle P4.7 (XOR)
        for(volatile uint32_t i = 0; i < k; i++);

        if (k >= 15000){
            k = 2500;
        }
        if (!(P1IN & BIT1)){    // Si P1.1 está a 0 (S2 pulsado)
            k *= 2;
        }
    }
    return 0;
}



void __attribute__((interrupt(PORT2_VECTOR))) P2_ISR(void) {
    if (P2IFG & BIT1) {	// comprobar fuente de la interrupción P1.1
        if (!(P2IN & BIT1)){    // Si P1.1 está a 0 (S1 pulsado)
            P1OUT ^= 0x01; // Toggle P1.0 (XOR)
        }
        P2IFG &= ~BIT1;	// puesta a cero del flag de interrupción P1.1
    }
}
