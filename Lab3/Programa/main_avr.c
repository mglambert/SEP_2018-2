// Mathias Lambert V. (c) - Lab3 SEP 2018-2
// Task 2
/*
    DDRx eligen si los pines a utilizar del microcontrolador AVR serán salidas
          digitales o entradas digitales x in {B, D}, 1 salida, 0 entrada

    PORTx, enciende 1 o apaga 0 salida para salidas
    PORTx, enciende 1 o apaga 0 resistencia de pull-up para entradas

    PINx lee los valores de entrada del puerto x
*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>

unsigned char val = (1 << 4);
unsigned char leer;
unsigned char leer2;
unsigned char estadoBoton;

unsigned char leer_2;
unsigned char leer2_2;
unsigned char estadoBoton_2;

int main(void) {
    estadoBoton = PIND & (1 << 3);
    estadoBoton_2 = PINC & (1 << 5);

    // Definimos los pines de salida
    DDRB |= (1 << 0);
    DDRB |= (1 << 1);
    DDRB |= (1 << 2);
    DDRB |= (1 << 3);
    DDRB |= (1 << 4);

    // Definimos la entrada del interruptor
    DDRB &= ~(1 << 7);
    PORTB |= (1 << 7);

    DDRD &= ~(1 << 3);
    PORTD |= (1 << 3);

    DDRC &= ~(1 << 5);
    PORTC |= (1 << 5);

    // Activar interrupcion
    PCMSK0 |= (1 << 7);
    PCICR |= (1 << 0);
    PCIFR = 0;

    PCMSK2 |= (1 << 3);
    PCICR |= (1 << 2);

    PCMSK1 |= (1 << 5);
    PCICR |= (1 << 1);



    PORTB = val;
    sei();

    while (1) {
        /* code */
    }
    return 0;
}


void led(unsigned char val) {
    PORTB &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
    PORTB |= val;
}


ISR(PCINT0_vect){
    if (!(PINB & (1 << 7))) {
        if (val == (1 << 0)){
            val = (1 << 4);
        }
        else {
            val = (val >> 1);
        }
        led(val);
    }
}


ISR(PCINT2_vect){
    leer = PIND & (1 << 3);
    _delay_ms(25);
    leer2 = PIND & (1 << 3);
    if (leer == leer2) {
        if (leer != estadoBoton) {
            if (leer == 0) {
                if (!(PIND & (1 << 3))) {
                    if (val == (1 << 4)) {
                        val = (1 << 0);
                    }
                    else {
                        val = (val << 1);
                    }
                    led(val);
                }
            }
        }
        estadoBoton = PIND & (1 << 3);
    }
}


ISR(PCINT1_vect){
    leer_2 = PINC & (1 << 5);
    _delay_ms(25);
    leer2_2 = PINC & (1 << 5);
    if (leer_2 == leer2_2) {
        if (leer_2 != estadoBoton_2) {
            if (leer_2 == 0) {
                if (!(PINC & (1 << 5))) {
                    for (int i = 0; i < 10; i++) {
                        val = (1 << 4) | (1 << 2) | (1 << 0);
                        led(val);
                        _delay_ms(500);
                        val = (1 << 3) | (1 << 1);
                        led(val);
                        _delay_ms(500);
                    }
                    for (int i = 0; i < 10; i++) {
                        val = (1 << 2);
                        led(val);
                        _delay_ms(200);
                        val = (1 << 3);
                        led(val);
                        _delay_ms(200);
                        val = (1 << 2);
                        led(val);
                        _delay_ms(200);
                        val = (1 << 1);
                        led(val);
                        _delay_ms(200);
                        val = (1 << 4);
                        led(val);
                        _delay_ms(200);
                        val = (1 << 0);
                        led(val);
                        _delay_ms(200);
                    }
                    for (int i = 0; i < 10; i++) {
                        val = (1 << 2);
                        led(val);
                        _delay_ms(200);
                        val = (1 << 1);
                        led(val);
                        _delay_ms(200);
                        val = (1 << 2);
                        led(val);
                        _delay_ms(200);
                        val = (1 << 3);
                        led(val);
                        _delay_ms(200);
                        val = (1 << 0);
                        led(val);
                        _delay_ms(200);
                        val = (1 << 4);
                        led(val);
                        _delay_ms(200);
                    }
                    val = (1 << 4);
                    led(val);
                }
            }
        }
        estadoBoton_2 = PINC & (1 << 5);
    }
}
