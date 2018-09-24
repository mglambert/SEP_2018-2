// Mathias Lambert V. (c) - Lab7 SEP 2018-2


#include "comunicacion/mi_uart.h"
#include <stdlib.h>


Lista* lista;
unsigned char recibido;
char* cadena_recibida;


int main(void) {
    lista = crear_lista();
    Configuracion* conf = crear_configuracion(38400, 0, 8, 1);
    configurar_uart(conf);
    kill_configuracion(conf);
    while(1);
}



#ifdef __AVR__
    #include <avr/io.h>
    #include <avr/interrupt.h>
    #define F_CPU 16000000UL

    ISR (USART_RX_vect) {
        recibido = UDR0;             // leer RX buffer
        if (recibido != 10 && recibido != 13) {
            agregar_nodo(lista, recibido);
        }
        else if (recibido == 10){
            cadena_recibida = leer_lista(lista);
            enviar_str("Se recibio: ");
            enviar_str(cadena_recibida);
            free(cadena_recibida);
            enviar_str(" - AVR\n");
            kill_lista(lista);
            lista = crear_lista();
        }
    }

#else
    #include <msp430.h>

    void enviar_dibujo(void) {
        enviar_str("                 /^ ^\\ \n");
        enviar_str("  __,_____      / 0 0 \\\n");
        enviar_str(" / __.==--\"     V\\ Y /V\n");
        enviar_str("/#(-'            / - \\ \n");
        enviar_str("`-'             /    | \n");
        enviar_str("               V__) || \n");
        enviar_str("Distincion o el perrito morira :( ");
    }

    void __attribute__((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR(void) {
      switch(__even_in_range(UCA1IV,4))
      {
      case 0:break;                             // Vector 0 - no interrupt
      case 2:
        while (!(UCA1IFG&UCTXIFG));              // Vector 2 - RXIFG
        // Recepcion de caracter
        recibido = UCA1RXBUF;
        if (recibido != 10 && recibido != 13) {
            agregar_nodo(lista, recibido);
        }
        else if (recibido == 10){
            cadena_recibida = leer_lista(lista);
            enviar_str("Se recibio: ");
            enviar_str(cadena_recibida);
            free(cadena_recibida);
            enviar_str(" - MSP\n");
            kill_lista(lista);
            lista = crear_lista();
            if (cadena_recibida[0] == 'a'){
                enviar_dibujo();
            }
        }

        break;
      case 4:break;                             // Vector 4 - TXIFG
      default: break;
      }
    }
#endif
