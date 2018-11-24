// Mathías Lambert (c) - Lab13 - SEP_2018-2
#include "./libreria/mi_uart.h"
#include "lab13.h"

enum estados {
    Cerrado,
    Abriendo,
    Abierto,
    Cerrando,
    ParadoAlCerrar,
    ParadoAlAbrir,
    ParadoIndefinido,
    Error,
    Inicio
}; typedef enum estados Estados;

Estados estado_actual;
Estados estado_siguiente;
int segundos = 0;   // son 0.25 segundos por unidad

int main() {
    // Iniciamos Pines
    Led_verde_INICIAR();
    Led_rojo_INICIAR();
    Boton_INICIAR();
    Motor_INICIAR();
    Sensor_derecha_INICIAR();
    Sensor_izquierda_INICIAR();

    // Iniciamos UART
    Configuracion* conf = crear_configuracion(57600, 0, 8, 1);
    configurar_uart(conf);
    kill_configuracion(conf);
    //enviar_str("Hola lab13\n");

    // Set Timer1s
    Set_timer();
    Desactivar_int_timer();

    Activar_interrupciones();

    // variables de estados
    estado_actual = Inicio;
    estado_siguiente = Inicio;

    // parametros de temporizacion
    int t1 = 7;
    int t2 = 25;
    int tp = 80;


    while (1) {
        estado_actual = estado_siguiente;
        //enviar_str("------\n");
        switch (estado_actual) {
            case Inicio:
                //enviar_str("Inicio\n");
                if (!(Leer_Sensor_derecha())) {
                    estado_siguiente = Cerrado;
                }
                else if (!(Leer_Sensor_izquierda())) {
                    estado_siguiente = Abierto;
                }
                else {
                    estado_siguiente = ParadoIndefinido;
                }
                break;

            case Cerrado:
                //enviar_str("Cerrado\n");
                Motor_OFF();
                Led_verde_ON();
                Led_rojo_OFF();
                int b = Leer_Boton();
                while (b) {
                    b = Leer_Boton();
                }
                estado_siguiente = Abriendo;
                break;

            case Abriendo:
                //enviar_str("Abriendo\n");
                Motor_ON();
                Led_verde_ON();
                Led_rojo_OFF();
                segundos = 0;
                Activar_int_timer();
                int b2 = Leer_Boton();
                int e;
                if (b2 == 0) {
                    //enviar_str("algo anda mal");
                }
                while(b2 && segundos < t2 && Leer_Sensor_izquierda()) {
                    b2 = Leer_Boton();
                    e = Leer_Sensor_derecha();
                    if (segundos > t1 && e == 0) {
                        break;
                    }
                }
                Desactivar_int_timer();
                if (b2 == 0) {
                    estado_siguiente = ParadoAlAbrir;
                }
                else if (segundos == t2 && Leer_Sensor_izquierda()) {
                    estado_siguiente = Error;
                }
                else if (!(Leer_Sensor_izquierda())) {
                    estado_siguiente = Abierto;
                }
                else if (e == 0) {
                    estado_siguiente = Error;
                }
                break;

            case Abierto:
                //enviar_str("Abierto\n");
                Motor_OFF();
                Led_verde_OFF();
                Led_rojo_ON();
                segundos = 0;
                Activar_int_timer();
                while(Leer_Boton() && segundos < tp);
                Desactivar_int_timer();
                estado_siguiente = Cerrando;
                break;

            case Cerrando:
                //enviar_str("Cerrando\n");
                Motor_ON();
                Led_verde_OFF();
                Led_rojo_ON();
                segundos = 0;
                Activar_int_timer();
                int b3 = Leer_Boton();
                int e2;
                while(b3 && segundos < t2 && Leer_Sensor_derecha()) {
                    b3 = Leer_Boton();
                    e2 = Leer_Sensor_izquierda();
                    if (segundos > t1 && e2 == 0) {
                        break;
                    }
                }
                Desactivar_int_timer();
                if (b3 == 0) {
                    estado_siguiente = ParadoAlCerrar;
                }
                else if (segundos == t2 && Leer_Sensor_derecha()) {
                    estado_siguiente = Error;
                }
                else if (!(Leer_Sensor_derecha())) {
                    estado_siguiente = Cerrado;
                }
                else if (e2 == 0) {
                    estado_siguiente = Error;
                }
                break;

            case ParadoAlAbrir:
                //enviar_str("ParadoAlAbrir\n");
                Motor_OFF();
                Led_verde_OFF();
                Led_rojo_ON();
                segundos = 0;
                Activar_int_timer();
                int b4 = Leer_Boton();
                while(b4 && segundos < tp) {
                    b4 = Leer_Boton();
                }
                Desactivar_int_timer();
                if (b4 == 0) {
                    estado_siguiente = ParadoIndefinido;
                    break;
                }
                estado_siguiente = Cerrando;
                break;

            case ParadoAlCerrar:
                //enviar_str("ParadoAlCerrar\n");
                Motor_OFF();
                Led_verde_ON();
                Led_rojo_ON();
                segundos = 0;
                Activar_int_timer();
                int b5 = Leer_Boton();
                while(b5 && segundos < tp) {
                    b5 = Leer_Boton();
                }
                Desactivar_int_timer();
                if (b5 == 0) {
                    estado_siguiente = ParadoIndefinido;
                    break;
                }
                estado_siguiente = Abriendo;
                break;

            case ParadoIndefinido:
                //enviar_str("ParadoIndefinido\n");
                Motor_OFF();
                Led_verde_OFF();
                Led_rojo_OFF();
                while(Leer_Boton());
                estado_siguiente = Cerrando;
                break;

            case Error:
                //enviar_str("Error\n");
                Led_rojo_ON();
                Led_verde_ON();
                Motor_OFF();
                while (Leer_Boton());
                estado_siguiente = Inicio;
                break;
        }
    }
}

// timer 0.25s
TimerISR() {
    switch (estado_actual) {
        case Abriendo:
            Led_rojo_xor();
            break;
        case Cerrando:
            Led_verde_xor();
            break;
        case ParadoAlAbrir:
            Led_rojo_xor();
            Led_verde_xor();
            break;
        case ParadoAlCerrar:
            Led_rojo_xor();
            Led_verde_xor();
            break;
        default:
            break;
    }
    segundos++;
}

ISR_RX() {
    #ifdef __MSP430__
    switch(__even_in_range(UCA1IV,4))
    {
    case 0:break;                             // Vector 0 - no interrupt
    case 2:
      while (!(UCA1IFG&UCTXIFG));              // Vector 2 - RXIFG
    #endif
    char recibido = charbuf;
    switch (estado_actual) {
        case Abierto:
            enviar_caracter('2');
            break;
        case Cerrado:
            enviar_caracter('0');
            break;
        case Abriendo:
            enviar_caracter('1');
            break;
        case Cerrando:
            enviar_caracter('3');
            break;
        case ParadoAlAbrir:
            enviar_caracter('4');
            break;
        case ParadoAlCerrar:
            enviar_caracter('4');
            break;
        case ParadoIndefinido:
            enviar_caracter('4');
            break;
        case Error:
            enviar_caracter('5');
            break;
        default:
            break;
    }
    #ifdef __MSP430__
    break;
    case 4:break;                             // Vector 4 - TXIFG
    default: break;
    }
    #endif
}
