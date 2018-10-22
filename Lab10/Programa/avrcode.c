// Mathías Lambert V. (c) - Lab10 SEP 2018-2

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#define F_CPU 16000000UL


void init_lab(void) {
    /* Timer 0 para contar tiempo
       modo comparación, cuenta hasta 249 para freq 1ms
       prescaler 64*/
    TCCR0A |= (1 << WGM01);
    OCR0A = 249;
    TCCR0B |= (1 << CS00) | (1 << CS01);

    /* Timer 1 para blink led
       modo comparación, cuenta hasta 7812 para freq 500ms
       prescaler 1024*/
    OCR1A = 7812;
    TCCR1B |= (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A);
    TCCR1B |= (1 << CS12) | (1 << CS10);
    DDRB |= (1 << 5);

    // Interruptor tarjeta
    DDRB &= ~(1 << 7);
    PORTB |= (1 << 7);
    PCMSK0 |= (1 << 7);
    PCICR |= (1 << 0);
    PCIFR = 0;

    // Interruptor aluminio
    DDRD &= ~(1 << 3);
    PORTD |= (1 << 3);
    PCMSK2 |= (1 << 3);
    PCICR |= (1 << 2);

    // Definimos los pines de salida
    DDRB |= (1 << 0);
    DDRB |= (1 << 1);
    DDRB |= (1 << 2);
    DDRB |= (1 << 3);
    DDRB |= (1 << 4);

    estadoBoton = PIND & (1 << 3);
    PORTB = val;

    sei();
    // enable interrupts

    enviar_str("Run Lab10\n");
}

void led(unsigned char val) {
    PORTB &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
    PORTB |= val;
}

ISR (TIMER0_COMPA_vect) {
    tiempo_presionado++;
    tiempo_presionado2++;
}

ISR (TIMER1_COMPA_vect) {
    PORTB ^= (1 << 5);
}

ISR(PCINT0_vect){
    if (!(PINB & (1 << 7))) {
        enviar_str("Boton tarjeta apretado\n");
        tiempo_presionado = 0;
        timer_en_uso++;
        TIMSK0 |= (1 << OCIE0A);
        if (val == (1 << 0)){
            val = (1 << 4);
        }
        else {
            val = (val >> 1);
        }
        led(val);
    }
    else if ((PINB & (1 << 7))) {
        timer_en_uso--;
        sprintf(msj, "Boton tarjeta presionado %d ms\n", tiempo_presionado);
        UCSR0B |= (1 << TXCIE0);
        //enviar_str(msj);
        if (timer_en_uso == 0) {
            TIMSK0 &= ~(1 << OCIE0A);
        }
    }
}

ISR(PCINT2_vect){
    leer = PIND & (1 << 3);
    _delay_ms(25);
    leer2 = PIND & (1 << 3);
    if (leer == leer2) {
        if (leer != estadoBoton) {
            if (leer == 0) {
                enviar_str("Boton aluminio apretado\n");
                TIMSK0 |= (1 << OCIE0A);
                tiempo_presionado2 = 0;
                timer_en_uso++;
                if (val == (1 << 4)) {
                    val = (1 << 0);
                }
                else {
                    val = (val << 1);
                }
                led(val);
            }
            if (leer) {
                timer_en_uso--;
                sprintf(msj, "Boton aluminio presionado %d ms\n", tiempo_presionado2);
                UCSR0B |= (1 << TXCIE0);
                //enviar_str(msj);
                if (timer_en_uso == 0) {
                    TIMSK0 &= ~(1 << OCIE0A);
                }
            }
        }
        estadoBoton = PIND & (1 << 3);
    }
}

ISR (USART_TX_vect) {
    enviar_str(msj);
    UCSR0B &= ~(1 << TXCIE0);
}
