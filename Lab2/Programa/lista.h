// Mathias Lambert V. (c) - Lab2 SEP 2018-2
#include <stdlib.h>
#include <stdint.h>


struct nodo {
    uint32_t delay;
    struct nodo * sig;
};
typedef struct nodo * Lista;

Lista insertar_final(Lista lista, uint32_t val) {
    struct nodo* aux;
    struct nodo* nuevo;
    nuevo = (struct nodo*) malloc(sizeof(struct nodo));
    nuevo -> delay = val;
    nuevo -> sig = NULL;
    if (lista == NULL) {
        lista = nuevo;
    }
    else {
        for (aux = lista; aux -> sig != NULL; aux = aux -> sig);
        aux -> sig = nuevo;
    }
    return lista;
}

Lista unir_cabeza_cola(Lista lista){
    struct nodo* aux;
    for (aux = lista; aux -> sig != NULL; aux = aux -> sig);
    aux -> sig = lista;
    return lista;
}
