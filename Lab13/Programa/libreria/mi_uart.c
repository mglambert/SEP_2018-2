// Mathías Lambert V. (c) - Lab8 SEP 2018-2

#include "mi_uart.h"
#include <stdlib.h>
#include <stdint.h>


Lista* crear_lista() {
    Lista* lista = malloc(sizeof(Lista));
    lista -> numero_de_datos = 0;
    lista -> cabeza = NULL;
    return lista;
}

void agregar_nodo(Lista* lista, unsigned char letra){
    lista -> numero_de_datos++;
    Nodo* nodo = malloc(sizeof(Nodo));
    nodo -> caracter = letra;
    nodo -> siguiente = NULL;
    if (lista -> cabeza == NULL) {
        lista -> cabeza = nodo;
    }
    else {
        Nodo* aux;
        for (aux = (lista -> cabeza); aux -> siguiente != NULL; aux = (aux -> siguiente));
        aux -> siguiente = nodo;
    }
}

void pasar_string(Lista* listarx, Lista* listatx) {
    Nodo* aux;
    Nodo* aux2;
    aux = listarx -> cabeza;
    while (aux != NULL){
        agregar_nodo(listatx, aux -> caracter);
        aux2 = aux;
        aux = aux -> siguiente;
        free(aux2);
    }
    free(listarx);
    listarx = crear_lista();
}

/*char* leer_lista(Lista* lista){
    if (lista -> cabeza != NULL){
        Nodo* aux;
        char* palabra = malloc((lista -> numero_de_datos + 1) * sizeof(char));
        int i = 0;
        aux = lista -> cabeza;
        while (aux != NULL){
            palabra[i] = aux -> caracter;
            i++;
            aux = aux -> siguiente;
        }
        palabra[i] = 0x00;
        return palabra;
    }
    else {
        return "";
    }
}*/

void kill_lista(Lista* lista){
    Nodo* aux1;
    Nodo* aux2;
    aux1 = lista -> cabeza;
    while (aux1 != NULL) {
        aux2 = aux1 -> siguiente;
        free(aux1);
        aux1 = aux2;
    }
    free(lista);
}

Configuracion* crear_configuracion(uint16_t baund, int bits_paridad, int bits_mensaje, int bist_parada) {
    Configuracion* conf = malloc(sizeof(Configuracion));
    conf -> baudrate = baund;
    conf -> bits_de_paridad = bits_paridad;
    conf -> bits_de_mensaje = bits_mensaje;
    conf -> bits_de_parada = bist_parada;
    return conf;
}

void kill_configuracion(Configuracion* configuracion) {
    free(configuracion);
}

#ifdef __AVR__
    #include <avr/io.h>
    #include <avr/interrupt.h>

    #define F_CPU 16000000UL

    void configurar_uart(Configuracion* datos) {

        uint8_t MYUBRR = F_CPU/16/(datos -> baudrate) - 1;

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

    void enviar_caracter(unsigned char caracter) {
        while(!(UCSR0A&(1<<5)));
        UDR0 = caracter;
    }

    void enviar_str(char* str) {
        while(*str !=0x00){
            while(!(UCSR0A&(1<<5)));
            UDR0 = *str;
            str++;
        }
    }

    void recibir_str(char* buffer, uint8_t bufflen) {
        char recibido;
        int c = 0;
        while(1){
            while ( !(UCSR0A & (1 << RXC0)) );
            recibido = UDR0;             // leer RX buffer
            if (c >= bufflen) {
                *buffer = 0x00;
                break;
            }
            if (recibido != 10 && recibido != 13) {
                *buffer = recibido;
            }
            else if (recibido == 10){
                *buffer = 0x00;
                break;
            }
            buffer++;
            c++;
        }
    }

    char recibir_char() {
        while ( !(UCSR0A & (1 << RXC0)) );
        return UDR0;
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
        switch (datos -> baudrate) {
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
        UCA1IE |= UCRXIE;                      // Enable USCI_A1 RX interrupt

    }

    void enviar_caracter(unsigned char caracter) {
        while(UCA1STAT&UCBUSY);
        UCA1TXBUF = caracter;
    }

    void enviar_str(char* str) {
        while(*str !=0x00){
            while(UCA1STAT&UCBUSY);
            UCA1TXBUF = *str;
            str++;
        }
    }

#endif

void leer_lista(Lista* lista){
    Nodo* aux;
    Nodo* aux2;
    aux = lista -> cabeza;
    while (aux != NULL){
        enviar_caracter(aux -> caracter);
        aux2 = aux;
        aux = aux -> siguiente;
        free(aux2);
    }
    free(lista);
}
