// Mathías Lambert V. (c) - Lab10 SEP 2018-2

#ifndef _MI_I2C_
#define _MI_I2C_

#include <stdint.h>

void i2c_iniciar();      //función para iniciar el módulo TWI I2C AVR
void i2c_espera();       //función de espera
void i2c_inicia_com();   //función que inicia la comunicación I2C AVR
void i2c_detener();      //función que detiene la comunicación I2C AVR
void i2c_envia_dato(unsigned char dato);   //función para enviar o escribir
                                       //datos en el esclavo
unsigned char i2c_recibe_dato_ack();   //función para recibir o leer datos del esclavo
                                       //enviando el bit ACK si se quiere leer mas datos
									   //después del último leído
unsigned char i2c_recibe_dato_nack();  //función para recibir o leer datos del esclavo
                                       //sin enviar el bit ACK si no se quiere leer mas datos
									   //después del último leido
uint8_t i2c_estado_com();  //función para averiguar el estado de la comunicación I2C AVR
                           //útil para detectar errores

#endif
