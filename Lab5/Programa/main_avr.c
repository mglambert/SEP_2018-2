// Mathías Lambert V.(c) - Lab5 SEP_2018-2

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD -1


unsigned char ReceivedChar;

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
    }
}

void enviar_str(char* str) {
    while(*str !=0x00){
        while(!(UCSR0A&(1<<5)));
        UDR0 = *str;
        str++;
    }
}
