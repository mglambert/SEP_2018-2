// Mathias Lambert V. (c) - Lab2 SEP 2018-2

#define F_CPU 16000000UL
#define LED_PIN 5
#include <avr/io.h>
#include <util/delay.h>
#include "lista.h"
#include <stdlib.h>
#include <stdint.h>



int main(void) {

    DDRB |= (1 << LED_PIN);
    Lista lista;
    lista = NULL;
    lista = insertar_final(lista, 20000);
    lista = insertar_final(lista, 20000);
    lista = insertar_final(lista, 30000);
    lista = insertar_final(lista, 30000);
    lista = insertar_final(lista, 50000);
    lista = insertar_final(lista, 50000);
    lista = insertar_final(lista, 70000);
    lista = insertar_final(lista, 70000);
    lista = insertar_final(lista, 110000);
    lista = insertar_final(lista, 110000);
    lista = insertar_final(lista, 130000);
    lista = insertar_final(lista, 130000);
    lista = insertar_final(lista, 170000);
    lista = insertar_final(lista, 170000);
    lista = insertar_final(lista, 130000);
    lista = insertar_final(lista, 130000);
    lista = insertar_final(lista, 110000);
    lista = insertar_final(lista, 110000);
    lista = insertar_final(lista, 70000);
    lista = insertar_final(lista, 70000);
    lista = insertar_final(lista, 50000);
    lista = insertar_final(lista, 50000);
    lista = insertar_final(lista, 30000);
    lista = insertar_final(lista, 30000);
    lista = unir_cabeza_cola(lista);

    while (1) {
        PORTB ^= (1 << LED_PIN);
        //_delay_ms(lista -> delay);
        for(volatile uint32_t i = 0; i < lista -> delay; i++);
        lista = lista -> sig;
    }

}
