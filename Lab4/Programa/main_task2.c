// Mathías Lambert V. (c) - Lab4 SEP 2018-2

#ifdef __MSP430__
    #include <msp430.h>
    #include <stdint.h>

    uint16_t temp;
    float vol;
    uint16_t T;
    unsigned char val;


    int main(void) {
        WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT


        P6DIR |= BIT5;        // Salida a Led P6.5
        P3DIR |= BIT4;        // Salida a Led P3.4
        P3DIR |= BIT3;        // Salida a Led P3.3
        P1DIR |= BIT6;        // Salida a Led P1.6
        P6DIR |= BIT6;        // Salida a Led P6.6

        ADC12CTL0 = ADC12SHT02 + ADC12ON;         // Sampling time, ADC12 on
        ADC12CTL1 = ADC12SHP;                     // Use sampling timer
        ADC12IE = 0x01;                           // Enable interrupt
        ADC12CTL0 |= ADC12ENC;
        P6DIR &= ~BIT0;
        P6SEL |= 0x01;                            // P6.0 ADC option select

        while(1) {
            ADC12CTL0 |= ADC12SC;                   // Sampling and conversion start

            __bis_SR_register(LPM0_bits + GIE);     // LPM0 with interrupts enabled

            vol = 3.3 * temp / 4096;
            //T = -32*vol + 75;
            T =  3.2 * vol * vol - 42.14 * vol + 81.63;

            P6OUT &= ~BIT5;
            P3OUT &= ~BIT4;
            P3OUT &= ~BIT3;
            P1OUT &= ~BIT6;
            P6OUT &= ~BIT6;

            if (T > 40) {
                val = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);

            }
            else if (T > 30) {
                val = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
            }
            else if (T > 20) {
                val = (1 << 0) | (1 << 1) | (1 << 2);
            }
            else if (T > 10) {
                val = (1 << 0) | (1 << 1);
            }
            else {
                val = (1 << 0);
            }
            P6OUT |= (BIT0 & val) << 5;
            P3OUT |= (BIT1 & val) << 3;
            P3OUT |= (BIT2 & val) << 1;
            P1OUT |= (BIT3 & val) << 3;
            P6OUT |= (BIT4 & val) << 2;

        __delay_cycles(50000);
        }
    }

    void __attribute__((interrupt(ADC12_VECTOR))) ADC12_ISR(void) {
        switch(__even_in_range(ADC12IV,34))
        {
        case  0: break;                           // Vector  0:  No interrupt
        case  2: break;                           // Vector  2:  ADC overflow
        case  4: break;                           // Vector  4:  ADC timing overflow
        case  6:                                  // Vector  6:  ADC12IFG0
        temp = ADC12MEM0;                       // Move results, IFG is cleared
        __bic_SR_register_on_exit(LPM0_bits);   // Exit active CPU
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

#else
    #include <avr/io.h>
    #include <util/delay.h>
    #include <avr/interrupt.h>
    #define F_CPU 16000000UL
    uint16_t valor = 0;
    unsigned char val = (1 << 0);

    int main(void) {
        float T = 0;
        float voltaje = 0;
        ADMUX = 0b01100011;
        cli();
        ADCSRA = 0b11001011;
        sei();
        ADCSRB = 0;
        DIDR0 |= (1 << 3);

        DDRB |= (1 << 0);
        DDRB |= (1 << 1);
        DDRB |= (1 << 2);
        DDRB |= (1 << 3);
        DDRB |= (1 << 4);

        while (1) {
            voltaje = valor*5.0/1023.0;
            T =  1.39 * voltaje * voltaje - 27.8 * voltaje + 81.6;
            if (T > 40) {
                val = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);

            }
            else if (T > 30) {
                val = (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
            }
            else if (T > 20) {
                val = (1 << 0) | (1 << 1) | (1 << 2);
            }
            else if (T > 10) {
                val = (1 << 0) | (1 << 1);
            }
            else {
                val = (1 << 0);
            }
            led(val);
        }
        return 0;
    }

    ISR(ADC_vect) {
        valor = ADC >> 6;
        ADCSRA |= (1 << ADSC);
    }

    void led(unsigned char val) {
        PORTB &= ~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4));
        PORTB |= val;
    }
#endif
