// Mathias Lambert V. (c) - Lab7 SEP 2018-2

#ifndef _MI_UART2_
#define _MI_UART2_

#include <stdint.h>
#include <stdlib.h>


struct configuracion {
    uint16_t baudrate;
    int bits_de_paridad;
    int bits_de_mensaje;
    int bits_de_parada;
}; typedef struct configuracion Configuracion;


struct nodo {
    char caracter;
    struct nodo* siguiente;
}; typedef struct nodo Nodo;


struct lista {
    int numero_de_datos;
    Nodo* cabeza;
}; typedef struct lista Lista;


Lista* crear_lista();

void agregar_nodo(Lista* lista, unsigned char letra);

char* leer_lista(Lista* lista);

void kill_lista(Lista* lista);


Configuracion* crear_configuracion(uint16_t baund, int bits_paridad, int bits_mensaje, int bist_parada);

void configurar_uart(Configuracion* datos);

void enviar_caracter(unsigned char caracter);

void enviar_str(char* str);

void kill_configuracion(Configuracion* configuracion);

#endif
