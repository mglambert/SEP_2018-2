// Mathías Lambert V. (c) - Lab9 SEP 2018-2

#include "./libreria/mi_uart.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef __AVR__
    #include <avr/io.h>
    #include <util/delay.h>
#else
    #include <msp430.h>
#endif


uint16_t dato;
int beta = 3470;
int R0 = 10;
int R = 18;
float V;
float T0 = 25 + 273.15;
float R_ntc;
float T;
char msj[50];



void init_adc(void);

int main(void) {
    Configuracion* conf = crear_configuracion(57600, 0, 8, 1);
    configurar_uart(conf);
    kill_configuracion(conf);
    init_adc();
    enviar_str("0\n");
    while (1){
        #ifdef __AVR__
            _delay_ms(2000);
            ADCSRA |= (1 << ADSC);
        #else
            for (volatile uint32_t k = 0; k < 60000; k++);
        #endif
    }
}


#ifdef __AVR__
    //#include <avr/io.h>
    #include <avr/interrupt.h>
    #define F_CPU 16000000UL

    void init_adc(void) {
        ADMUX = 0;                                                  // uso de ADC0
        ADMUX |= (0 << REFS0);                                      // Referencia ARef
        ADMUX |= (0 << ADLAR);

        ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);       // 128 prescale para 16Mhz
        ADCSRA |= (0 << ADATE);                                     // Set ADC Auto Trigger Enable

        ADCSRB = 0;                                                 // modo libre

        ADCSRA |= (1 << ADEN);                                      // Activar ADC
        ADCSRA |= (1 << ADIE);                                      // Activar interrupcion

        //ADCSRA |= (1 << ADSC);                                      // Iniciar conversion
        sei();
    }

    ISR(ADC_vect) {
        //dato = (ADCH << 8) + ADCL;
        dato = ADC;
        UCSR0B |= (1 << TXCIE0);
    }

    ISR (USART_TX_vect) {
        V = dato * 3.3 / 1023.0;
        R_ntc = V * R / (5 - V);
        T = beta / (log(R_ntc / R0) + beta/T0) - 273.15;
        //sprintf(msj, "%d\n", (int)T);
        //enviar_str(msj);
        char T2;
        T2 = (char) T;
        enviar_caracter(T2);
        UCSR0B &= ~(1 << TXCIE0);

    }

#else
    //#include <msp430.h>

    void init_adc(void) {
        P6SEL |= BIT0;                            // P6.0 ADC option select
        ADC12CTL0 = ADC12ON + ADC12SHT0_8 + ADC12MSC;
        ADC12CTL1 = ADC12SHP + ADC12CONSEQ_2;
        ADC12IE = 0x01;                           // Enable interrupt
        ADC12CTL0 |= ADC12ENC;
        ADC12CTL0 |= ADC12SC;
        __bis_SR_register(GIE);
    }

    void __attribute__((interrupt(ADC12_VECTOR))) ADC12_ISR(void) {
        switch(__even_in_range(ADC12IV,34))
        {
        case  0: break;                           // Vector  0:  No interrupt
        case  2: break;                           // Vector  2:  ADC overflow
        case  4: break;                           // Vector  4:  ADC timing overflow
        case  6:                                  // Vector  6:  ADC12IFG0
        dato = ADC12MEM0;
        UCA1IE |= UCTXIE;
        case  8: break;                           // Vector  8:  ADC12IFG1
        case 10: break;                           // Vector 10:  ADC12IFG2
        case 12: break;                           // Vector 12:  ADC12IFG3
        case 14: break;                           // Vector 14:  ADC12IFG4
        case 16: break;                           // Vector 16:  ADC12IFG5
        case 18: break;                           // Vector 18:  ADC12IFG6
        case 20: break;                           // Vector 20:  ADC12IFG7
        case 22: break;                           // Vector 22:  ADC12IFG8
        case 24: break;                           // Vector 24:  ADC12IFG9
        case 26: break;                           // Vector 26:  ADC12IFG10
        case 28: break;                           // Vector 28:  ADC12IFG11
        case 30: break;                           // Vector 30:  ADC12IFG12
        case 32: break;                           // Vector 32:  ADC12IFG13
        case 34: break;                           // Vector 34:  ADC12IFG14
        default: break;
        }
    }

    void __attribute__((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR(void) {
      switch(__even_in_range(UCA1IV,4)) {
      case 0:break;                             // Vector 0 - no interrupt
      case 2:                                   // Vector 2 - RXIFG
        break;
      case 4:                              // Vector 4 - TXIFG

        //sprintf(msj, "%d\n", dato);
        //enviar_str(msj);
        char T2;
        T2 = (char) T;
        enviar_caracter(T2);
        UCA1IE &= ~UCTXIE;
        break;
      default: break;
      }
    }
#endif
