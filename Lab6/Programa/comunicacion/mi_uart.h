// Mathias Lambert V. (c) - Lab6 SEP 2018-2


#ifndef _MI_UART_
#define _MI_UART_
#include <stdint.h>


struct configuracion {

    uint16_t baudios;
    int bits_de_paridad;
    int bits_de_mensaje;
    int bits_de_parada;

}; typedef struct configuracion Configuracion;


Configuracion* ingresar_datos(uint16_t baudios, int bits_de_paridad,
    int bits_de_mensaje, int bits_de_parada);

void configurar_uart(Configuracion* datos);

void enviar_caracter(char caracter);

void enviar_string(char* str);

void enviar_salto_de_linea(void);

void enviar_dibujo(void);

void kill_configuracion(Configuracion* configuracion);

void probar_comunicacion(uint16_t bound);

void probar_comunicacion_pedida(void);


#endif
