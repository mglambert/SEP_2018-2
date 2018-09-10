// Mathías Lambert V.(c) - Lab5 SEP_2018-2

#ifdef __MSP430__

    #include <msp430.h>

    char recibido;
    char *mensaje1 = "Se recibio un: ";
    char *mensaje2 = " - Cadena enviada del msp43f5529";
    char *cadena = "Se recibieron y enviaron todas las letras! - Cadena enviada del msp430f5529";


    int main(void) {
        WDTCTL = WDTPW + WDTHOLD;         // Detener WDT

        P4SEL = BIT5 + BIT4;              // P4.5,4 = USCI_A1 TXD/RXD
        UCA1CTL1 |= UCSWRST;              // **Put state machine in reset**
        UCA1CTL1 |= UCSSEL_2;             // CLK = SMCLK
        UCA1BR0 = 17;                   // 1MHz/57600=17.3 (see User's Guide)
        UCA1BR1 = 0x00;
        UCA1MCTL = UCBRS_3 + UCBRF_0;    // Modulation UCBRSx=3, UCBRFx=0

        UCA1CTL0 &= ~UCPEN;
        UCA1CTL0 &= ~UC7BIT;

        UCA1CTL1 &= ~UCSWRST;                  // **Initialize USCI state machine**
        UCA1IE |= UCRXIE;                      // Enable USCI_A1 RX interrupt

        __bis_SR_register(GIE);
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

#else

    #include <avr/io.h>
    #include <avr/interrupt.h>

    #define F_CPU 16000000UL
    #define BAUD 57600
    #define MYUBRR F_CPU/16/BAUD -1


    unsigned char ReceivedChar;
    void enviar_str(char* cadena);
    void enviar_dibujo(void);

    int main(void) {
        DDRB |= (1 << 5);

        /*Fijar bound rate*/
        UBRR0H = (MYUBRR >> 8);
        UBRR0L = MYUBRR;

        UCSR0B |= (1 << RXEN0) | (1 << TXEN0);      // Activar recepcion y transmision
        UCSR0B |= (1 << RXCIE0);                    // Activar interrupcion para recibir
        //UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00) | (1 << USBS0);  // 8data, 2 stp
        UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);  // 8data, 1 stp

        sei();

        while(1){}
    }

    ISR (USART_RX_vect)
    {
        PORTB ^= (1 << 5);
        ReceivedChar = UDR0;             // leer RX buffer
        enviar_str("Se recibio: ");

        // Enviar caracter
        while(!(UCSR0A&(1<<5)));
        UDR0 = ReceivedChar;             // escribirTX buffer

        //while(!(UCSR0A&(1<<5)));
        //UDR0 = 13;                       // retorno de carro

        enviar_str(" - Cadena enviada del atmega328p");

        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;                       // nueva linea
        // Enviar cadena
        if (ReceivedChar == 'z'){
            enviar_str("Se recibieron y enviaron todas las letras - Cadena enviada del atmega328p");
            while(!(UCSR0A&(1<<5)));
            UDR0 = 10;
            enviar_dibujo();
        }
    }

    void enviar_str(char* str) {
        while(*str !=0x00){
            while(!(UCSR0A&(1<<5)));
            UDR0 = *str;
            str++;
        }
    }

    void enviar_dibujo(void) {
        enviar_str("                 /^ ^\\ ");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_str("  __,_____      / 0 0 \\");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_str(" / __.==--\"     V\\ Y /V");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_str("/#(-'            / - \\ ");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_str("`-'             /    | ");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_str("               V__) || ");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_str("Distincion o el perrito morira :( ");

    }

#endif
