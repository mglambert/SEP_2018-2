// Mathías Lambert V.(c) - Lab5 SEP_2018-2

#include <msp430.h>

char recibido;
char *mensaje1 = "Se recibio un: ";
char *mensaje2 = " - Cadena enviada del msp43f5529";
char *cadena = "Se recibieron y enviaron todas las letras! - Cadena enviada del msp430f5529";


int main(void) {
    WDTCTL = WDTPW + WDTHOLD;         // Detener WDT

    P4SEL = BIT5 + BIT4;              // P4.5,4 = USCI_A1 TXD/RXD
    UCA1CTL1 |= UCSWRST;              // **Put state machine in reset**
    UCA1CTL1 |= UCSSEL_1;             // CLK = ACLK
    UCA1BR0 = 0x03;                   // 32kHz/9600=3.41 (see User's Guide)
    UCA1BR1 = 0x00;
    UCA1MCTL = UCBRS_3 + UCBRF_0;    // Modulation UCBRSx=3, UCBRFx=0

    UCA1CTL0 &= ~UCPEN;
    UCA1CTL0 &= ~UC7BIT;

    UCA1CTL1 &= ~UCSWRST;                  // **Initialize USCI state machine**
    UCA1IE |= UCRXIE;                      // Enable USCI_A1 RX interrupt

    __bis_SR_register(LPM3_bits + GIE);    // Enter LPM3, interrupts enabled
    __no_operation();
}


void __attribute__((interrupt(USCI_A1_VECTOR))) USCI_A0_ISR(void) {
  switch(__even_in_range(UCA1IV,4))
  {
  case 0:break;                             // Vector 0 - no interrupt
  case 2:
    while (!(UCA1IFG&UCTXIFG));              // Vector 2 - RXIFG
    // Recepcion de caracter
    recibido = UCA1RXBUF;

    // Enviar mensaje1
    while(*mensaje1 !=0x00){
        while(UCA1STAT&UCBUSY);
        UCA1TXBUF = *mensaje1;
        mensaje1++;
    }
    mensaje1 = "Se recibio un: ";

    // Enviar caracter recibido
    while(UCA1STAT&UCBUSY);
    UCA1TXBUF = recibido;

    // Enviar mensaje2
    while(*mensaje2 !=0x00){
        while(UCA1STAT&UCBUSY);
        UCA1TXBUF = *mensaje2;
        mensaje2++;
    }
    mensaje2 = " - Cadena enviada del msp43f5529";

    // Enviar salto de linea
    while(UCA1STAT&UCBUSY);
    UCA1TXBUF = 10;

    // Enviar cadena final
    if (recibido == 'z'){
        while(*cadena !=0x00){
            while(UCA1STAT&UCBUSY);
            UCA1TXBUF = *cadena;
    		cadena++;
    	}
        cadena = "Se recibieron y enviaron todas las letras! - Cadena enviada del msp430f5529";
    }
    break;
  case 4:break;                             // Vector 4 - TXIFG
  default: break;
  }
}
