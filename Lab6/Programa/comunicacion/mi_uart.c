// Mathias Lambert V. (c) - Lab6 SEP 2018-2

#include "mi_uart.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

Configuracion* ingresar_datos(uint16_t baud, int bits_paridad,
    int bits_mensaje, int bits_parada) {

    Configuracion* configuracion = malloc(sizeof(Configuracion));

    configuracion -> baudios = baud;
    configuracion -> bits_de_paridad = bits_paridad;
    configuracion -> bits_de_mensaje = bits_mensaje;
    configuracion -> bits_de_parada = bits_parada;
    return configuracion;
}

void kill_configuracion(Configuracion* configuracion) {
    free(configuracion);
}

#ifdef __AVR__
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #include <util/delay.h>
    #include <avr/eeprom.h>
    #define F_CPU 16000000UL
    uint8_t EEMEM num = 0;

    void configurar_uart(Configuracion* datos) {

        uint8_t MYUBRR = F_CPU/16/(datos -> baudios) - 1;
        UCSR0C = 0;
        UCSR0B = 0;
        UBRR0H = 0;
        UBRR0L = 0;

        /*Fijar bound rate*/
        UBRR0H = (MYUBRR >> 8);
        UBRR0L = MYUBRR;

        UCSR0B |= (1 << RXEN0) | (1 << TXEN0);      // Activar recepcion y transmision
        UCSR0B |= (1 << RXCIE0);                    // Activar interrupcion para recibir
        switch (datos -> bits_de_mensaje) {
            case 5:
                UCSR0C |= (0 << UCSZ01) | (0 << UCSZ00);  // 5data
                break;
            case 6:
                UCSR0C |= (0 << UCSZ01) | (1 << UCSZ00);  // 6data
                break;
            case 7:
                UCSR0C |= (1 << UCSZ01) | (0 << UCSZ00);  // 7data
                break;
            case 8:
                UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);  // 8data
                break;
            default:
                UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);  // 8data
                break;

        }

        if (datos -> bits_de_parada == 2) {
            UCSR0C |= (1 << USBS0);  // 2bits de parada
        }

        switch (datos -> bits_de_paridad) {
            case 1:
                UCSR0C |= (1 << UPM01) | (0 << UPM00);  // Even parity
                break;
            case 2:
                UCSR0C |= (1 << UPM01) | (1 << UPM00);  // Odd parity
                break;
            default:
                UCSR0C |= (0 << UPM01) | (0 << UPM00);  // Disabled parity
                break;
        }
    }

    void enviar_caracter(char caracter) {
        while(!(UCSR0A&(1<<5)));
        UDR0 = caracter;
    }

    void enviar_string(char* str) {
        while(*str !=0x00){
            while(!(UCSR0A&(1<<5)));
            UDR0 = *str;
            str++;
        }
    }

    void enviar_salto_de_linea(void) {
        enviar_caracter(10);
    }

    void enviar_dibujo(void) {
        enviar_string("                 /^ ^\\ ");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_string("  __,_____      / 0 0 \\");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_string(" / __.==--\"     V\\ Y /V");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_string("/#(-'            / - \\ ");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_string("`-'             /    | ");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_string("               V__) || ");
        while(!(UCSR0A&(1<<5)));
        UDR0 = 10;
        enviar_string("Distincion o el perrito morira :( ");
    }

    void probar_comunicacion(uint16_t bound) {
        char mensaje[50];
        char p;
        Configuracion* conf = NULL;
        int digit1 = bound/1000;
        int digit2 = bound - digit1*1000;
        for (int datos = 7; datos < 9; datos++) {
            for (int par = 0; par < 2; par++) {
                for (int stop = 1; stop < 3; stop++) {
                    switch (par) {
                        case 0:
                            p = 'N';
                            break;
                        case 1:
                            p = 'E';
                            break;
                        case 2:
                            p = 'O';
                            break;
                        default:
                            p = 'x';
                            break;
                    }
                    conf = ingresar_datos(bound, par, datos, stop);
                    configurar_uart(conf);
                    num++;
                    sprintf(mensaje, "\nHola Mundo!!! - Configuracion %d%d %d%c%d en AVR. Mensajes enviados %d.\r\n", digit1, digit2, datos, p, stop, num);
                    enviar_string(mensaje);
                    _delay_ms(50);
                }
            }
        }
        kill_configuracion(conf);
    }

    void  probar_comunicacion_pedida(void) {
        Configuracion* conf = NULL;
        char mensaje[50];
        uint8_t total;
        conf = ingresar_datos(57600, 0, 8, 1);
        configurar_uart(conf);
        //enviar_string("\nhola!, configuracion 57600 8N1 en AVR.\r\n");
        total = eeprom_read_byte(&num);
        total++;
        eeprom_write_byte(&num, total);
        sprintf(mensaje, "\nHola Mundo!!! - Configuracion 57600 8N1 en AVR. Mensajes enviados %d.\r\n", total);
        enviar_string(mensaje);

        conf = ingresar_datos(9600, 0, 8, 2);
        configurar_uart(conf);
        //enviar_string("\nhola!, configuracion 9600 8N2 en AVR\r\n");
        total = eeprom_read_byte(&num);
        total++;
        eeprom_write_byte(&num, total);
        sprintf(mensaje, "\nHola Mundo!!! - Configuracion 9600 8N2 en AVR. Mensajes enviados %d.\r\n", total);
        enviar_string(mensaje);

        conf = ingresar_datos(19200, 0, 7, 1);
        configurar_uart(conf);
        //enviar_string("\nhola!, configuracion 19200 7N1 en AVR\r\n");
        total = eeprom_read_byte(&num);
        total++;
        eeprom_write_byte(&num, total);
        sprintf(mensaje, "\nHola Mundo!!! - Configuracion 19200 7N1 en AVR. Mensajes enviados %d.\r\n", total);
        enviar_string(mensaje);

        conf = ingresar_datos(19200, 1, 7, 2);
        configurar_uart(conf);
        //enviar_string("\nhola!, configuracion 19200 7E2 en AVR\r\n");
        total = eeprom_read_byte(&num);
        total++;
        eeprom_write_byte(&num, total);
        sprintf(mensaje, "\nHola Mundo!!! - Configuracion 19200 7E2 en AVR. Mensajes enviados %d.\r\n", total);
        enviar_string(mensaje);
        kill_configuracion(conf);
        while(1);
    }

#else
    #include <msp430.h>

    void configurar_uart(Configuracion* datos) {
        WDTCTL = WDTPW + WDTHOLD;         // Detener WDT

        // SMCLK = 12MHz
        UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
        UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

        __bis_SR_register(SCG0);                  // Disable the FLL control loop
        UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
        UCSCTL1 = DCORSEL_5;                      // Select DCO range 24MHz operation
        UCSCTL2 = FLLD_1 + 374;                   // Set DCO Multiplier for 12MHz
                                                   // (N + 1) * FLLRef = Fdco
                                                   // (374 + 1) * 32768 = 12MHz
                                                   // Set FLL Div = fDCOCLK/2
        __bic_SR_register(SCG0);                  // Enable the FLL control loop

        // Worst-case settling time for the DCO when the DCO range bits have been
        // changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
        // UG for optimization.
        // 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
        __delay_cycles(375000);


        P4SEL = BIT5 + BIT4;              // P4.5,4 = USCI_A1 TXD/RXD
        UCA1CTL1 |= UCSWRST;              // **Put state machine in reset**
        UCA1CTL1 |= UCSSEL_2;             // CLK = SMCLK

        UCA1BR1 = 0x00;
        switch (datos -> baudios) {
            case 9600:
                UCA1BR0 = 226;                   // 12MHz/9600=1250 (see User's Guide)
                UCA1BR1 = 4;
                UCA1MCTL = UCBRS_0 + UCBRF_0;    // Modulation UCBRSx=1, UCBRFx=0
                break;

            case 19200:
                UCA1BR0 = 113;                   // 12MHz/19200=625 (see User's Guide)
                UCA1BR1 = 2;
                UCA1MCTL = UCBRS_0 + UCBRF_0;    // Modulation UCBRSx=0, UCBRFx=0
                break;

            case 38400:
                UCA1BR0 = 56;                   // 12MHz/38400=312.4 (see User's Guide)
                UCA1BR1 = 1;
                UCA1MCTL = UCBRS_4 + UCBRF_0;    // Modulation UCBRSx=0, UCBRFx=0
                break;

            case 57600:
                UCA1BR0 = 208;                   // 12MHz/57600=208.2 (see User's Guide)
                UCA1MCTL = UCBRS_2 + UCBRF_0;    // Modulation UCBRSx=3, UCBRFx=0
                break;

            default:
                UCA1BR0 = 208;                   // 12MHz/57600=208.2 (see User's Guide)
                UCA1MCTL = UCBRS_2 + UCBRF_0;    // Modulation UCBRSx=3, UCBRFx=0
                break;
        }

        switch (datos -> bits_de_paridad) {
            case 1:
                UCA1CTL0 |= UCPEN;
                UCA1CTL0 |= UCPAR;
                break;
            case 2:
                UCA1CTL0 |= UCPEN;
                UCA1CTL0 &= ~UCPAR;
                break;
            default:
                UCA1CTL0 &= ~UCPEN;
                break;
        }

        if (datos -> bits_de_mensaje == 7) {
            UCA1CTL0 |= UC7BIT;
        }
        else {
            UCA1CTL0 &= ~UC7BIT;
        }

        if (datos -> bits_de_parada == 1) {
            UCA1CTL0 &= ~UCSPB;
        }
        else {
            UCA1CTL0 |= UCSPB;
        }

        UCA1CTL1 &= ~UCSWRST;                  // **Initialize USCI state machine**
    }

    void enviar_caracter(char caracter) {
        while(UCA1STAT&UCBUSY);
        UCA1TXBUF = caracter;
    }

    void enviar_string(char* str) {
        while(*str !=0x00){
            while(UCA1STAT&UCBUSY);
            UCA1TXBUF = *str;
            str++;
        }
    }

    void enviar_salto_de_linea(void) {
        enviar_caracter(10);
    }

    void enviar_dibujo(void) {
        enviar_string("                 /^ ^\\ ");
        enviar_salto_de_linea();
        enviar_string("  __,_____      / 0 0 \\");
        enviar_salto_de_linea();
        enviar_string(" / __.==--\"     V\\ Y /V");
        enviar_salto_de_linea();
        enviar_string("/#(-'            / - \\ ");
        enviar_salto_de_linea();
        enviar_string("`-'             /    | ");
        enviar_salto_de_linea();
        enviar_string("               V__) || ");
        enviar_salto_de_linea();
        enviar_string("Distincion o el perrito morira :( ");
    }

    void probar_comunicacion(uint16_t bound) {
        char mensaje[50];
        char p;
        Configuracion* conf = NULL;
        int digit1 = bound/1000;
        int digit2 = bound - digit1*1000;
        for (int datos = 7; datos < 9; datos++) {
            for (int par = 0; par < 2; par++) {
                for (int stop = 1; stop < 3; stop++) {
                    switch (par) {
                        case 0:
                            p = 'N';
                            break;
                        case 1:
                            p = 'E';
                            break;
                        case 2:
                            p = 'O';
                            break;
                        default:
                            p = 'x';
                            break;
                    }
                    conf = ingresar_datos(bound, par, datos, stop);
                    configurar_uart(conf);
                    sprintf(mensaje, "\nHola Mundo!!! - Configuracion %d%d %d%c%d en MSP.\r\n", digit1, digit2, datos, p, stop);
                    enviar_string(mensaje);
                    for(volatile int i = 0; i < 70; i++);
                }
            }
        }
        kill_configuracion(conf);
    }
    void  probar_comunicacion_pedida() {
        Configuracion* conf = ingresar_datos(57600, 0, 8, 1);
        configurar_uart(conf);
        enviar_string("                       \r\n");
        enviar_string("\nhola!, configuracion 57600 8N1 en MSP.\n");
        for(volatile int i = 0; i < 50; i++);

        conf = ingresar_datos(9600, 0, 8, 2);
        configurar_uart(conf);
        enviar_string("\nhola!, configuracion 9600 8N2 en MSP\n");
        for(volatile int i = 0; i < 50; i++);

        conf = ingresar_datos(19200, 0, 7, 1);
        configurar_uart(conf);
        enviar_string("\nhola!, configuracion 19200 7N1 en MSP\n");
        for(volatile int i = 0; i < 50; i++);

        conf = ingresar_datos(19200, 1, 7, 2);
        configurar_uart(conf);
        enviar_string("\nhola!, configuracion 19200 7E2 en MSP\n");
        kill_configuracion(conf);
        while (1);
    }

#endif
