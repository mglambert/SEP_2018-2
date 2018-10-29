// Mathías Lambert V. (c) - Lab11 SEP 2018-2

#include "./libreria/mi_uart.h"
#include "./libreria/mi_i2c.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lib/st7735.h"

#define PI 3.14
// read and write addresses; set ADD0 = GND
#define TMP102_READ	 	0x91
#define TMP102_WRITE 	0x90
#define F_CPU 16000000UL


bool menu = true;
bool mostrar_t = false;
char recibido;
float temperatura = 0.0;

int entero;
int decimal;

enum funcion {
    sinusoide,
    casa,
    triangular,
    sierra
}; typedef enum funcion mode;

mode modo = sinusoide;

uint16_t housetab[512] =
{
    0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,  3083,  3083,  3083,  3083,
3083,  3083,  3083,  3083,  3083,  3083,
3083,  3083,  3083,  3083,  3083,  3083,
2569,  2569,  2569,  2569,  2569,  2569,
2569,  2569,  2569,  2569,  2569,  2569,
2569,  2569,  2569,  2569,  2569,  2569,
2569,  2569,  2569,  2569,  2569,  2569,
2569,  2569,  2569,  2569,  2569,  2569,
2569,  2569,  3083,  3083,  3083,  3083,
3083,  3083,  3083,  3083,  3083,  3083,
3083,  3083,  3083,  3083,  3083,  3083,
2056,  2056,  2056,  2056,  2056,  2056,
2056,  2056,  2056,  2056,  2056,  2056,
2056,  2056,  2056,  2056,  2056,  2056,
2056,  2056,  2056,  2056,  2056,  2056,
2056,  2056,  2056,  2056,  2056,  2056,
2056,  2056,  3097,  3130,  3162,  3194,
3226,  3258,  3290,  3323,  3355,  3387,
3419,  3451,  3484,  3516,  3548,  3580,
3612,  3644,  3677,  3709,  3741,  3773,
3805,  3838,  3870,  3902,  3934,  3966,
3998,  4031,  4063,  4095,  4095,  4063,
4031,  3998,  3966,  3934,  3902,  3870,
3838,  3805,  3773,  3741,  3709,  3677,
3644,  3612,  3580,  3548,  3516,  3484,
3451,  3419,  3387,  3355,  3323,  3290,
3258,  3226,  3194,  3162,  3130,  3097,
2056,  2056,  2056,  2056,  2056,  2056,
2056,  2056,  2056,  2056,  2056,  2056,
2056,  2056,  2056,  2056,  2056,  2056,
2056,  2056,  2056,  2056,  2056,  2056,
2056,  2056,  2056,  2056,  2056,  2056,
2056,  2056,  3083,  3083,  3083,  3083,
3083,  3083,  3083,  3083,  3083,  3083,
3083,  3083,  3083,  3083,  3083,  3083,
2569,  2569,  2569,  2569,  2569,  2569,
2569,  2569,  2569,  2569,  2569,  2569,
2569,  2569,  2569,  2569,  2569,  2569,
2569,  2569,  2569,  2569,  2569,  2569,
2569,  2569,  2569,  2569,  2569,  2569,
2569,  2569,  3083,  3083,  3083,  3083,
3083,  3083,  3083,  3083,  3083,  3083,
3083,  3083,  3083,  3083,  3083,  3083,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0,     0,     0,     0,     0,
   0,     0
};

char m[30];
void enviar_dac_house();
void enviar_dac_sierra();
void enviar_dac_tri();
void enviar_dac_sin();
float leer_temp();
void pantalla(char* str1, char* str2);
char m2[20] = "Sinusoide";

int main(void) {
    sprintf(m, "00.00 Celsius");
    Configuracion* conf = crear_configuracion(57600, 0, 8, 1);
    configurar_uart(conf);
    kill_configuracion(conf);

    i2c_iniciar();
    enviar_str("Lab 11\n");
    UCSR0B |= (1 << TXCIE0);
    UCSR0B &= ~(1 << RXCIE0);
    sei();
    St7735Init();
    pantalla(m, m2);
    while (1){
        switch (modo) {
            case sinusoide:
                enviar_dac_sin();
                break;
            case casa:
                enviar_dac_house();
                break;
            case triangular:
                enviar_dac_tri();
                break;
            case sierra:
                enviar_dac_sierra();
                break;
            default:
                enviar_dac_sin();
                break;
        }
        if (mostrar_t) {
            temperatura = leer_temp();
            entero = (int) temperatura;
            decimal = (int)((temperatura - entero)*100);
            sprintf(m, "\t\t\t\t\t\t| Temperatura %d.%dC |\n", entero, decimal);
            enviar_str("\t\t\t\t\t\t---------------------\n");
            enviar_str(m);
            enviar_str("\t\t\t\t\t\t---------------------\n");
            sprintf(m, "%d.%d Celsius", entero, decimal);
            pantalla(m, m2);
            mostrar_t = false;
            UCSR0B |= (1 << TXCIE0);
        }
    }
}

void mcp_envia_dato(uint16_t dato) {
    i2c_inicia_com();
    i2c_espera();
    i2c_envia_dato(0xc0);
    char p1 = (char)((dato & 0xF00) >> 8);
    i2c_envia_dato(p1);
    i2c_envia_dato((dato & 0xFF));
    i2c_detener();
}

void enviar_dac_house() {
    static int in = 0;
    mcp_envia_dato(housetab[in]);
    in++;
    if (in == 512) {
        in = 0;
    }
}

void enviar_dac_sierra() {
    static uint16_t sierra = 0;
    mcp_envia_dato(sierra);
    sierra = sierra + 10;
    if (sierra == 4000) {
        sierra = 0;
    }
}

void enviar_dac_tri() {
    static uint16_t tri = 0;
    static bool sube = true;
    mcp_envia_dato(tri);
    if (sube){
        tri = tri + 40;
    }
    else {
        tri = tri - 40;
    }

    if (tri == 4000 || tri == 0) {
        sube = !sube;
    }
}

void enviar_dac_sin() {
    static uint16_t cuenta = 0;
    double p = 2047*sin(PI*2*cuenta/4000)+2047;
    uint16_t dato = (uint16_t) p;
    mcp_envia_dato(dato);
    cuenta = cuenta + 10;
    if (cuenta == 4000) {
        cuenta = 0;
    }
}

float leer_temp(void) {

	uint8_t msb, lsb;
	float t;
    uint16_t temp;


	i2c_inicia_com();							// send start condition

	i2c_envia_dato(TMP102_WRITE); 				// send WRITE address of TMP102
	i2c_envia_dato(0x00); 						// set TMP102 pointer register to 0 (read temperature)

	i2c_inicia_com();							// send repeated start condition

	i2c_envia_dato(TMP102_READ);				// send READ address of TMP102
	msb = i2c_recibe_dato_ack(); 			// reads the MSB (it is a 12 bit value!)
	lsb = i2c_recibe_dato_nack();				// reads the LSB

	i2c_detener();							// send stop condition


	// Convert the number to an 8-bit degree Celsius value
	temp = (msb<<8) | lsb;					// combine those two values into one 16 bit value
	temp >>= 4; 							// the values are left justified; fix that by shifting 4 right
	// negative numbers are represented in two's complement, but we just shifted the value and thereby potentially messed it up
	if(temp & (1<<11))						// Hence: if it is a negative number
		temp |= 0xF800;						// restore the uppermost bits

	// The 12 bit value has 0.0625°C precision, which is too much for what we want (and the sensor is anyways only about 0.5°C precise)
	// 0.0625 is 1/16 -> Dividing by 16 leaves 1°C precision for the output. Note that shifting >> 4 might go wrong here for negative numbers.
	t = 0.0625 * temp;

	return(t);
}



void menu1() {
    enviar_str("\n\n*******************************\n");
    enviar_str("Selecciones una operacion:\n");
    enviar_str("\t1) Obtener temperatura\n");
    enviar_str("\t2) Seleccionar funcion DAC\n");
}

void menu2() {
    enviar_str("-----------\n");
    enviar_str("Selecciones una funcion:\n");
    enviar_str("\t1) Castillo\n");
    enviar_str("\t2) Sierra\n");
    enviar_str("\t3) Triangular\n");
    enviar_str("\t4) Sinusoidal\n");
}

ISR (USART_RX_vect) {
    recibido = UDR0;             // leer RX buffer
    if (menu) {
        if (recibido == '1') {
            mostrar_t = true;
            UCSR0B &= ~(1 << RXCIE0);
        }
        else {
            menu = false;
            UCSR0B |= (1 << TXCIE0);
            UCSR0B &= ~(1 << RXCIE0);
        }
    }
    else {
        switch (recibido) {
            case '1':
                modo = casa;
                sprintf(m2, "Castillo");
                break;
            case '2':
                modo = sierra;
                sprintf(m2, "Sierra");
                break;
            case '3':
                modo = triangular;
                sprintf(m2, "Triangular");
                break;
            case '4':
                modo = sinusoide;
                sprintf(m2, "Sinusoide");
                break;
            default:
                modo = sinusoide;
                sprintf(m2, "Sinusoide");
                break;

        }
        pantalla(m, m2);
        menu = true;
        UCSR0B |= (1 << TXCIE0);
        UCSR0B &= ~(1 << RXCIE0);
    }
}

ISR (USART_TX_vect) {
    if (menu) {
        menu1();
    }
    else {
        menu2();
    }
    UCSR0B &= ~(1 << TXCIE0);
    UCSR0B |= (1 << RXCIE0);
}

void pantalla(char* str1, char* str2) {
    ClearScreen(0xFFFF);

  // draw line
  DrawLine(23, 137, 40, 40, 0x301F);
  // draw line
  DrawLine(23, 137, 41, 41, 0x301F);

  DrawLine(23, 137, 55, 55, 0x301F);
  // draw line
  DrawLine(23, 137, 56, 56, 0x301F);

  DrawLine(23, 23, 40, 56, 0xC01F);
  DrawLine(24, 24, 40, 56, 0xC01F);
  DrawLine(137, 137, 40, 56, 0xC01F);
  DrawLine(136, 136, 40, 56, 0xC01F);
  // set text position
  SetPosition(10, 30);
  // draw text
  DrawString("Ultima Temperatura Leida", 0x0000, X1);
  // set text position
  SetPosition(48, 46);
  // draw text
  DrawString(str1, 0x0000, X1);
  // display on

  // draw line
  DrawLine(23, 137, 40+40, 40+40, 0xF800);
  // draw line
  DrawLine(23, 137, 41+40, 41+40, 0xF800);

  DrawLine(23, 137, 55+40, 55+40, 0xF800);
  // draw line
  DrawLine(23, 137, 56+40, 56+40, 0xF800);

  DrawLine(23, 23, 40+40, 56+40, 0xF800);
  DrawLine(24, 24, 40+40, 56+40, 0xF800);
  DrawLine(137, 137, 40+40, 56+40, 0xF800);
  DrawLine(136, 136, 40+40, 56+40, 0xF800);
  // set text position
  SetPosition(10, 30+40);
  // draw text
  DrawString("Funcion en DAC", 0x0000, X1);
  // set text position
  SetPosition(48, 46+40);
  // draw text
  DrawString(str2, 0x0000, X1);
  // display on


  UpdateScreen();
}
