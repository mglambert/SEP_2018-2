// Mathias Lambert V.(c) - SEP 2018-2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./estructuras/estructuras.h"
#include "./estructuras/printDibujo.c"

int main() {
    printf("Hola Mundo!!\n\n");
    sleep(1);
    printf("Ahora jugaremos al ahorcado\n");
    printf("%d\n", 3);
    sleep(1);
    printf("%d\n", 2);
    sleep(1);
    printf("%d\n", 1);
    sleep(1);
    printf("%s\n", "YA!");
    sleep(1);
    int vidas;
    int match;
    int win;
    char letra;
    vidas = 6;
    match = 0;
    Lista* lista;
    win = 0;
    lista = NULL;
    struct Nodo* aux;
    lista = insertar_final(lista, 'a');
    lista = insertar_final(lista, 'g');
    lista = insertar_final(lista, 'o');
    lista = insertar_final(lista, 's');
    lista = insertar_final(lista, 't');
    lista = insertar_final(lista, 'o');
    while (1) {
        system("clear");
        printDibujo(vidas);
        printf("----------------------------\n");
        palabra_a_ver(lista);
        printf("----------------------------\n");
        if (win == 6) {
            printf("WIN!!\n");
            break;
        };
        printf("\nIngrese una letra: \n");
        scanf(" %c", &letra);
        aux = lista;
        win = 0;
        match = 0;
        while(aux != NULL){
            if (aux -> valor == letra && aux -> mostrar == 0){
                aux -> mostrar = 1;
                match = 1;
            };
            win = win + aux -> mostrar;
            aux = aux -> sig;
        };
        if (match == 0){
            vidas--;
        };
        if (vidas == 0) {
            break;
        };
    };
    borrar(lista);
    return 0;
}
