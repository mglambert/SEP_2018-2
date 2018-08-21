#include "lista.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>



int main(int argc, char const *argv[]) {
    Lista lista;
    lista = NULL;
    lista = insertar_final(lista, 500);
    lista = insertar_final(lista, 1000);
    lista = insertar_final(lista, 1100);
    lista = insertar_final(lista, 1200);
    lista = insertar_final(lista, 1300);
    lista = unir_cabeza_cola(lista);

    for (int i = 0; i < 16; i++) {
        printf("%d\n", lista -> delay);
        lista = lista -> sig;

    }
    return 0;
}
