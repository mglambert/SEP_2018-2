// Mathías Lambert (c) - Lab13 - SEP_2018-2
#ifdef __AVR__
#include <avr/io.h>
#include <avr/interrupt.h>


#define F_CPU 16000000UL

// Definicion de Leds
#define Led_verde PB0
#define Led_rojo PB1

#define Led_verde_INICIAR() DDRB |= (1 << Led_verde)
#define Led_rojo_INICIAR() DDRB |= (1 << Led_rojo)

#define Led_verde_ON() PORTB |= (1 << Led_verde)
#define Led_rojo_ON() PORTB |= (1 << Led_rojo)

#define Led_verde_OFF() PORTB &= ~(1 << Led_verde)
#define Led_rojo_OFF() PORTB &= ~(1 << Led_rojo)

#define Led_verde_xor() PORTB ^= (1 << Led_verde)
#define Led_rojo_xor() PORTB ^= (1 << Led_rojo)


// Definicion de boton
#define Boton PB2

#define Boton_INICIAR() {DDRB &= ~(1 << Boton); PORTB |= (1 << Boton);}

//#define Boton_INT_ON() {PCICR |= (1 << PCIE0); PCMSK0 |= (1 << PCINT2);}

//#define Boton_INT_OFF() PCICR &= ~(1 << PCIE0)

//#define BotonISR() ISR(PCINT0_vect)

#define leer_boton_sub() PINB & (1 << Boton)


// Definicion motor
#define Motor PB3

#define Motor_INICIAR() DDRB |= (1 << Motor)

#define Motor_ON() PORTB |= (1 << Motor)

#define Motor_OFF() PORTB &= ~(1 << Motor)


// Sensores de carrera
#define Sensor_izquierda PB4
#define Sensor_derecha PD7

#define Sensor_izquierda_INICIAR() {DDRB |= ~(1 << PB4); PORTB |= (1 << PB4);}
#define Sensor_derecha_INICIAR() {DDRD |= ~(1 << PD7); PORTD |= (1 << PD7);}

#define Leer_Sensor_izquierda() PINB & (1 << PB4)
#define Leer_Sensor_derecha() PIND & (1 << PD7)


// Interrupciones UART
#define ISR_RX() ISR (USART_RX_vect)
#define ISR_TX() ISR (USART_TX_vect)

#define Activar_TX() UCSR0B |= (1 << TXCIE0)
#define Desactivar_TX() UCSR0B &= ~(1 << TXCIE0)

#define Activar_RX() UCSR0B |= (1 << RXCIE0)
#define Desactivar_RX() UCSR0B &= ~(1 << RXCIE0)

#define charbuf UDR0

// Activar Interrupciones
#define Activar_interrupciones() sei()


// Timer 0.25 seg
#define Set_timer() {OCR1A = 0x3D08; TCCR1B |= (1 << WGM12); TCCR1B |= (1 << CS12);}

#define Activar_int_timer() TIMSK1 |= (1 << OCIE1A)

#define Desactivar_int_timer() TIMSK1 &= ~(1 << OCIE1A)

#define TimerISR() ISR(TIMER1_COMPA_vect)

#else //-------------------------------------------MSP--------------------------
#include <msp430.h>

// Definicion de Leds
#define Led_verde_INICIAR() P3DIR |= BIT4 //P3.4
#define Led_rojo_INICIAR() P3DIR |= BIT3  //P3.3

#define Led_verde_ON() P3OUT |= BIT4
#define Led_rojo_ON() P3OUT |= BIT3

#define Led_verde_OFF() P3OUT &= ~(BIT4)
#define Led_rojo_OFF() P3OUT &= ~(BIT3)

#define Led_verde_xor() P3OUT ^= BIT4
#define Led_rojo_xor() P3OUT ^= BIT3


// Definicion de boton
#define Boton_INICIAR() {P1REN |= BIT6; P1OUT |= BIT6;}  //P1.6

#define leer_boton_sub() P1IN & BIT6


// Definicion motor
#define Motor_INICIAR() P6DIR |= BIT6 //P6.6

#define Motor_ON() P6OUT |= BIT6

#define Motor_OFF() P6OUT &= ~(BIT6)


// Sensores de carrera
#define Sensor_izquierda_INICIAR() {P3REN |= BIT2; P3OUT |= BIT2;}
#define Sensor_derecha_INICIAR() {P2REN |= BIT7; P2OUT |= BIT7;}

#define Leer_Sensor_izquierda() P3IN & BIT2
#define Leer_Sensor_derecha() P2IN & BIT7


// Interrupciones UART ---------------------------------------------------------------
#define ISR_RX() void __attribute__((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR(void)
//#define ISR_TX() ISR (USART_TX_vect)

#define Activar_TX() UCA1IE |= UCTXIE;
#define Desactivar_TX() UCA1IE &= ~(UCTXIE)

#define Activar_RX() UCA1IE |= UCRXIE;
#define Desactivar_RX() UCA1IE &= ~(UCRXIE)

#define charbuf UCA1RXBUF


// Activar Interrupciones
#define Activar_interrupciones() __enable_interrupt()


// Timer 0.25 seg
#define Set_timer() {TA1CTL = TASSEL_1 | TACLR | MC_1; TA1CCR0 = 8191;}

#define Activar_int_timer() TA1CCTL0 |= CCIE;

#define Desactivar_int_timer() TA1CCTL0 &= ~(CCIE);

#define TimerISR() void __attribute__((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR(void)


#endif


// Funciones debouncing boton

int leer_boton() {
    if (leer_boton_sub()){
        return 1;
    }
    return 0;
}

int Leer_Boton() {
    static int estado = 1;
    int lectura1 = leer_boton();
    for(volatile int i = 0; i < 2500; i++);
    int lectura2 = leer_boton();
    if (lectura1 == lectura2) {
        if (lectura1 != estado) {
            if (lectura1 == 0) {
                estado = 0;
                return 0;
            }
            estado = 1;
        }
    }
    return 1;
}
