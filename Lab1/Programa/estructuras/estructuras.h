// Mathias Lambert V.(c) - SEP 2018-2
/*
    Se implementa una lista ligada con tal de realizar el juego del
    ahorcado

          ________________________
         |                        |
         |                        |
         |                      -----
         |                     | O O |
         |                     |  -  |
         |                      -----
         |                        |
         |                   ---- | ----
         |                        |
         |                       ---
         |                      |   |
         |                      |   |
         |                      |   |
         |
    _____|_____
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct Nodo {
    char valor;
    struct Nodo* sig;
    int mostrar;
};
typedef struct Nodo Lista;

Lista* insertar_final(Lista* lista, char val) {
    struct Nodo* aux;
    struct Nodo* nuevo;
    nuevo = (struct Nodo*) malloc(sizeof(struct Nodo));
    nuevo -> valor = val;
    nuevo -> sig = NULL;
    nuevo -> mostrar = 0;
    if (lista == NULL) {
        lista = nuevo;
    }
    else {
        for (aux = lista; aux -> sig != NULL; aux = aux -> sig);
        aux -> sig = nuevo;
    }
    return lista;
}

Lista* borrar_cabeza(Lista* lista) {
    struct Nodo* aux;
    if (lista != NULL) {
        aux = lista -> sig;
        free(lista);
        lista = aux;
    }
    return lista;
}

void borrar(Lista* lista) {
    while(lista != NULL) {
        lista = borrar_cabeza(lista);
    }
}

void palabra_a_ver(Lista* lista) {
    struct Nodo* aux;
    aux = lista;
    while (aux != NULL) {
        if (aux -> mostrar == 0) {
            printf("%c", '_');
        }
        else {
            printf("%c", aux -> valor);
        }
        aux = aux -> sig;
    }
    printf("\n");
}
