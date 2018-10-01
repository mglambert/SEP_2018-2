// Mathías Lambert V. (c) - Lab8 SEP 2018-2

#include "./libreria/mi_uart.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Lista* listaRX;
Lista* listaTX;
unsigned char recibido;
void init_timer_boton(void);

int main(void) {
    listaRX = crear_lista();
    listaTX = crear_lista();
    Configuracion* conf = crear_configuracion(38400, 0, 8, 1);
    configurar_uart(conf);
    kill_configuracion(conf);
    init_timer_boton();
    enviar_str("Hola\n");
    while (1);
}



#ifdef __AVR__
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #include <avr/eeprom.h>
    #define F_CPU 16000000UL

    uint16_t EEMEM vida = 0;
    uint16_t vida_total;

    void init_timer_boton(void) {
        // Activar interrucion por desbordamiento
        TIMSK1 |= (1 << TOIE1);

        // Prescaler de 1024 -> time1_f = 15625Hz
        TCCR1B |= (1 << CS12) | (1 << CS10);

        // Seteamos el registro TCN1 para que la cuenta hasta el desbordamiento sea de 1s
        TCNT1 = 49911;

        // Definimos la entrada del interruptor
        DDRB &= ~(1 << 7);
        PORTB |= (1 << 7);

        // Activamos interrupcion del boton de la placa
        PCMSK0 |= (1 << 7);
        PCICR |= (1 << 0);
        PCIFR = 0;
        sei();
    }

    ISR (TIMER1_OVF_vect) {
        TCNT1 = 49911;
        vida_total = eeprom_read_word(&vida);
        vida_total++;
        eeprom_write_word(&vida, vida_total);
    }

    ISR (USART_RX_vect) {
        recibido = UDR0;             // leer RX buffer
        if (recibido != 10 && recibido != 13) {
            agregar_nodo(listaRX, recibido);
        }
        else if (recibido == 10){
            pasar_string(listaRX, listaTX);
            UCSR0B |= (1 << TXCIE0);
            UCSR0B &= ~(1 << RXCIE0);
        }
    }

    ISR (USART_TX_vect) {
        enviar_str("Se recibio: ");
        leer_lista(listaTX);
        enviar_str(" - AVR\n");
        listaTX = crear_lista();
        UCSR0B &= ~(1 << TXCIE0);
        UCSR0B |= (1 << RXCIE0);
    }

    ISR(PCINT0_vect){
        if (!(PINB & (1 << 7))) {
            char mensaje[50];
            vida_total = eeprom_read_word(&vida);
            int v = (int) vida_total;
            float minutos = vida_total/60.0;
            int d = (int) minutos;
            int dd = (minutos - d)*1000;
            sprintf(mensaje, "Tiempo desde la compilacion: %d segundos o %d.%d minutos\n", v, d, dd);
            enviar_str(mensaje);
        }
    }
#else
    #include <msp430.h>
    int vida = 0;

    void init_timer_boton(void) {
        // Configuración TIMER_A:
        // TimerA1, ACLK/8,modo up, reinicia TACLR
        TA1CTL = TASSEL_1 | TACLR | MC_1 | ID_3;
        // ACLK tiene una frecuencia de 32768 Hz
        TA1CCR0 = 4095;
        TA1CCTL0 = CCIE; // Habilita interrupción (bit CCIE)

        P1REN |= BIT1;	      // Resistencia activa en P1.1 (S2)
        P1OUT |= BIT1;	      // Configurada como pull-up

        P1IE |= BIT1;		  // Habilita la interrupción de P1.1
        P1IES |= BIT1;	      // Interrupción por flanco de bajada
        P1IFG &= ~(BIT1);	  // Baja flag de interrupción de P1.1

        __bis_SR_register(GIE);
    }

    void __attribute__((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR(void) {
        vida++;
    }

    void __attribute__((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR(void) {
      switch(__even_in_range(UCA1IV,4)) {
      case 0:break;                             // Vector 0 - no interrupt
      case 2:                                   // Vector 2 - RXIFG
        while (!(UCA1IFG&UCTXIFG));
        // Recepcion de caracter
        recibido = UCA1RXBUF;
        if (recibido != 10 && recibido != 13) {
            agregar_nodo(listaRX, recibido);
        }
        else if (recibido == 10){
            pasar_string(listaRX, listaTX);
            UCA1IE |= UCTXIE;
        }
        break;
      case 4:                              // Vector 4 - TXIFG
        enviar_str("Se recibio: ");
        leer_lista(listaTX);
        enviar_str(" - MSP\n");
        listaTX = crear_lista();
        UCA1IE &= ~UCTXIE;
        break;
      default: break;
      }
    }

    void __attribute__((interrupt(PORT1_VECTOR))) P1_ISR(void) {
        if (P1IFG & BIT1) {	// comprobar fuente de la interrupción P1.1
            if (!(P1IN & BIT1)){    // Si P1.1 está a 0 (S2 pulsado)
                char mensaje[70];
                float minutos = vida/60.0;
                int d = (int) minutos;
                int dd = (minutos - d)*1000;
                sprintf(mensaje, "Tiempo de funcionamiento: %d segundos o %d.%d minutos\n", vida, d, dd);
                enviar_str(mensaje);
            }
            P1IFG &= ~BIT1;	// puesta a cero del flag de interrupción P1.1
        }
    }

#endif
