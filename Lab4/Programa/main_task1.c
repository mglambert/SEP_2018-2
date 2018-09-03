// Mathías Lambert V. (c) - Lab4 SEP 2018-2

#ifdef __MSP430__
    #include <msp430.h>

    int main(void) {
        WDTCTL = WDTPW + WDTHOLD;

        P1DIR |= BIT0;        // Port1 direction register, Pin0 as output

        P1SEL |= 0x04;
        P1DIR |= 0x04;

        TA0CCTL1 = OUTMOD_7;
        TA0CCR0 = 64;
        TA0CCR1 = 0;
        TA0CTL = TASSEL_1 + MC_1;

        // TimerA1, ACLK/1,modo up, reinicia TACLR
        TA1CTL = TASSEL_1 | TACLR | MC_1;
        // ACLK tiene una frecuencia de 32768 Hz
        TA1CCR0 = 3000;
        TA1CCTL0 = CCIE; // Habilita interrupción (bit CCIE)


        _low_power_mode_0();
        __no_operation();
    }
    void __attribute__((interrupt(TIMER1_A0_VECTOR))) TIMER1_A0_ISR(void) {
        if (TA0CCR1 < 64) {
            TA0CCR1 += 1;
        }
        else {
            TA0CCR1 = 0;
        }
    }

#else
    #define F_CPU 16000000UL
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #include <util/delay.h>
    int modo = 0;
    int up = 1;
    int main(void) {
        DDRD |= (1 << DDD6);
        // PD6 is now an output

        OCR0A = 1;
        // set PWM for 50% duty cycle


        TCCR0A |= (1 << COM0A1);
        // set none-inverting mode

        TCCR0A |= (1 << WGM01) | (1 << WGM00);
        // set fast PWM Mode

        TCCR0B |= (1 << CS01);
        // set prescaler to 8 and starts PWM

        PCMSK0 |= (1 << 7);
        PCICR |= (1 << 0);
        PCIFR = 0;
        sei();


        while (1) {
            if (modo == 0){
                if (OCR0A < 255) {
                    OCR0A++;
                }
                else {
                    OCR0A = 0;
                }
            }
            else {
                if (up == 1) {
                    if (OCR0A < 255) {
                        OCR0A++;
                    }
                    if (OCR0A == 255) {
                        up = 0;
                    }
                }
                else{
                    if (OCR0A > 0) {
                        OCR0A--;
                    }
                    if (OCR0A == 0) {
                        up = 1;
                    }
                }
            }
            _delay_ms(20);
        }
    }

    ISR(PCINT0_vect){
        if (!(PINB & (1 << 7))) {
            if (modo == 0) {
                modo = 1;
                OCR0A = 0;
                up = 1;
            }
            else {
                modo = 0;
            }
        }
    }

#endif
