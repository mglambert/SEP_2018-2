// Mathías Lambert V. (c) - Lab10 SEP 2018-2

int tiempo_presionado = 0;
int tiempo_presionado2 = 0;
int timer_en_uso = 0;
char msj[40];
unsigned char val = (1 << 4);
unsigned char leer;
unsigned char leer2;
unsigned char estadoBoton;

#ifdef __AVR__
    #include "avrcode.c"
#else
    #include "mspcode.c"
#endif
