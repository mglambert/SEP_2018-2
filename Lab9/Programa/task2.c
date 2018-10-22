#include "libreria/mi_uart.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void init_pwms();
int pow2(int b);
Lista* lista = NULL;
char recibido;
char* value;
int T = 22;
int n;

char msj[50];

int main(void) {
    lista = crear_lista();
    Configuracion* conf = crear_configuracion(57600, 0, 8, 1);
    configurar_uart(conf);
    kill_configuracion(conf);
    init_pwms();
    while(1);
}

int pow2(int b) {
    /*int x = 0;
    for (int i = 0; i < b; i++) {
        x += a;
    }*/
    if (b == 0) {
        return 1;
    }
    return 10;
}

#ifdef __MSP430__
    #include <msp430.h>

    void init_pwms() {
        P2DIR = BIT0 | BIT4 | BIT5 | BIT2;
        P1DIR = BIT4 | BIT5;
        P2SEL = BIT0 | BIT4 | BIT5;
        P1SEL = BIT4 | BIT5;

        TA2CCR0 = 64;
        TA2CTL = TASSEL_1 + ID_0 + MC_1;
        TA2EX0 = TAIDEX_0;
        TA2CCTL2 = OUTMOD_7;
        TA2CCR2 = 0;
        TA2CCTL1 = OUTMOD_7;
        TA2CCR1 = 0;

        TA0CCR0 = 64;
        TA0CTL = TASSEL_1 + ID_0 + MC_1;
        TA0EX0 = TAIDEX_0;
        TA0CCTL4 = OUTMOD_7;
        TA0CCR4 = 0;
        TA0CCTL3 = OUTMOD_7;
        TA0CCR3 = 0;

        TA1CCR0 = 64;
        TA1CTL = TASSEL_1 + ID_0 + MC_1;
        TA1EX0 = TAIDEX_0;
        TA1CCTL1 = OUTMOD_7;
        TA1CCR1 = 0;
        __bis_SR_register(GIE);
    }

    void encender_leds(int T) {
        if (T < 0) {
            TA2CCR2 = 0;
            TA2CCR1 = 0;
            TA0CCR4 = 0;
            TA0CCR3 = 0;
            TA1CCR1 = 0;
            P2OUT &= BIT2;
        }
        else if (0 <= T && T< 10) {
            TA2CCR2 = (int) (T * 64/10);
            TA2CCR1 = 0;
            TA0CCR4 = 0;
            TA0CCR3 = 0;
            TA1CCR1 = 0;
            P2OUT &= ~BIT2;
        }
        else if (10 <= T && T < 20) {
            TA2CCR2 = 64;
            TA2CCR1 = (int) ((T-9) * 64/10);
            TA0CCR4 = 0;
            TA0CCR3 = 0;
            TA1CCR1 = 0;
            P2OUT &= ~BIT2;
        }
        else if (20 <= T && T < 25) {
            TA2CCR2 = 64;
            TA2CCR1 = 64;
            TA0CCR4 = (int) ((T-19) * 64/5);
            TA0CCR3 = 0;
            TA1CCR1 = 0;
            P2OUT &= ~BIT2;
        }
        else if (25 <= T && T < 30) {
            TA2CCR2 = 64;
            TA2CCR1 = 64;
            TA0CCR4 = 64;
            TA0CCR3 = (int) ((T-24) * 64/5);
            TA1CCR1 = 0;
            P2OUT &= ~BIT2;
        }
        else if (30 <= T && T < 40) {
            TA2CCR2 = 64;
            TA2CCR1 = 64;
            TA0CCR4 = 64;
            TA0CCR3 = 64;
            TA1CCR1 = (int) ((T-29) * 64/10);
            P2OUT &= ~BIT2;
        }
        else {
            TA2CCR2 = 64;
            TA2CCR1 = 64;
            TA0CCR4 = 64;
            TA0CCR3 = 64;
            TA1CCR1 = 64;
            P2OUT |= BIT2;
        }
    }

    void __attribute__((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR(void) {
      switch(__even_in_range(UCA1IV,4)) {
      case 0:break;                             // Vector 0 - no interrupt
      case 2:                                   // Vector 2 - RXIFG
        while (!(UCA1IFG&UCTXIFG));
        // Recepcion de caracter
        recibido = UCA1RXBUF;
        T = (int) recibido;
        encender_leds(T);
        /*if (recibido != 10 && recibido != 13) {
            agregar_nodo(lista, recibido);
        }
        else if (recibido == 10){
            n = lista -> numero_de_datos;
            value = leer_lista2(lista);
            T = 0;
            while(*value !=0x00){
                T += pow2(n-1) * ((int)*value - 48);
                n--;
                value++;
            }
            encender_leds(T);
            sprintf(msj, "T: %d \n", T);
            kill_lista(lista);
            lista = crear_lista();
            enviar_str(msj);

        }*/
        break;
      case 4:                              // Vector 4 - TXIFG
        break;
      default: break;
      }
    }

#else
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #define F_CPU 16000000UL

    void init_pwms() {

        DDRD |= (1 << DDD6) | (1 << 5);
        // PD6  and PD5 is now an output
        OCR0A = 0;
        OCR0B = 0;
        // set PWM for 0% duty cycle
        TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
        // set none-inverting mode
        TCCR0A |= (1 << WGM01) | (1 << WGM00);
        // set fast PWM Mode
        TCCR0B |= (1 << CS02);
        // set prescaler to 256 and starts PWM

        DDRB |= (1 << DDB1)|(1 << DDB2);
        // PB1 and PB2 is now an output
        ICR1 = 255;
        // set TOP to 16bit
        OCR1A = 0;
        OCR1B = 0;
        // set PWM for 0% duty cycle
        TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
        // set none-inverting mode
        TCCR1A |= (1 << WGM11);
        TCCR1B |= (1 << WGM12)|(1 << WGM13);
        // set Fast PWM mode using ICR1 as TOP
        TCCR1B |= (1 << CS12);
        // START the timer with no prescaler

        DDRD |= (1 << DDD3);
        DDRB |= (1 << DDB3);
        // PD3 and PB3 is now an output
        OCR2A = 0;
        OCR2B = 0;
        // set PWM for 0% duty cycle
        TCCR2A |= (1 << COM2A1) | (1 << COM2B1);
        // set none-inverting mode
        TCCR2A |= (1 << WGM21) | (1 << WGM20);
        // set fast PWM Mode
        TCCR2B |= (1 << CS21) | (1 << CS22);
        // set prescaler to 256 and starts PWM
        sei();
    }

    void encender_leds(int T) {
        if (T < 0) {
            OCR0A = 0;
            OCR0B = 0;
            OCR1A = 0;
            OCR1B = 0;
            OCR2A = 0;
            OCR2B = 0;
        }
        else if (0 <= T && T< 10) {
            OCR0A = (int) ((T) * 255/10);
            OCR0B = 0;
            OCR1A = 0;
            OCR1B = 0;
            OCR2A = 0;
            OCR2B = 0;
        }
        else if (10 <= T && T < 20) {
            OCR0A = 255;
            OCR0B = (int) ((T-9) * 255/10);
            OCR1A = 0;
            OCR1B = 0;
            OCR2A = 0;
            OCR2B = 0;
        }
        else if (20 <= T && T < 25) {
            OCR0A = 255;
            OCR0B = 255;
            OCR1A = (int) ((T-19) * 255/5);
            OCR1B = 0;
            OCR2A = 0;
            OCR2B = 0;
        }
        else if (25 <= T && T < 30) {
            OCR0A = 255;
            OCR0B = 255;
            OCR1A = 255;
            OCR1B = (int) ((T-24) * 255/5);
            OCR2A = 0;
            OCR2B = 0;
        }
        else if (30 <= T && T < 40) {
            OCR0A = 255;
            OCR0B = 255;
            OCR1A = 255;
            OCR1B = 255;
            OCR2A = (int) ((T-29) * 255/10);
            OCR2B = 0;
        }
        else {
            OCR0A = 255;
            OCR0B = 255;
            OCR1A = 255;
            OCR1B = 255;
            OCR2A = 255;
            OCR2B = 255;
        }
    }

    ISR (USART_RX_vect) {
        recibido = UDR0;             // leer RX buffer
        T = (int) recibido;
        encender_leds(T);
        /*if (recibido != 10 && recibido != 13) {
            if (recibido >= 48 && recibido < 58){
                agregar_nodo(lista, recibido);
            }
        }
        else if (recibido == 10){
            //UCSR0B &= ~(1 << RXCIE0);
            int n2;
            n = lista -> numero_de_datos;
            n2 = n;
            char* v2;
            value = leer_lista2(lista);
            v2 = value;
            kill_lista(lista);
            lista = crear_lista();
            T = 0;
            int v;
            while(*value != 0x00){
                v = (int) *value;
                //*value = 0x00;
                if (v >= 48 && v < 58){
                    T += pow2(n-1) * (v - 48);
                }
                else{
                    *value = 0x00;
                }
                n--;
                value++;
            }
            encender_leds(T);
            sprintf(msj, "T: %d - %s - %d\n", T, v2, n2);
            enviar_str(msj);
            //UCSR0B |= (1 << RXCIE0);
        }*/
    }

#endif
