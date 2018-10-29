// Mathías Lambert V. (c) - Lab10 SEP 2018-2

#include "mi_i2c.h"
#include <stdint.h>
#include <avr/io.h>

void i2c_iniciar() {
	PORTC |= ((1 << 4) | (1 << 5));  //activa resistencias pull upp para SCL y SDA
	TWBR = 12;                       //velocidad 400Khz, Fosc 16Mhz, prescaler de 1
	TWCR |= (1 << TWEN);            //módulo TWI iniciado
}

void i2c_espera() {
	while (!(TWCR & (1 << TWINT))); // espera mientras el  bit de interrupcion sea 0
}

void i2c_inicia_com() {
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);//bandera de interrupción a 1, start, habilita I2C AVR
	i2c_espera();       //espera mientras el bit TWINT sea 0
}

void i2c_detener() {
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);//bandera de interrupción a 1, detener, habilita I2C AVR
}

void i2c_envia_dato(unsigned char dato) {
	TWDR = dato;
	TWCR = (1<<TWINT)|(1<<TWEN);//para empezar a enviar el dato
	i2c_espera();//cuando TWINT se ponga a 1 se habrá terminado de enviar el dato
}

unsigned char i2c_recibe_dato_ack() {//maestro envia ack para seguir recibiendo
	                                //mas  datos
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	i2c_espera();
	return TWDR;
}
unsigned char i2c_recibe_dato_nack() {//maestro no envia ack para no seguir recibiendo
	                                 //mas  datos
	TWCR = (1<<TWINT)|(1<<TWEN);
	i2c_espera();
	return TWDR;
}

uint8_t i2c_estado_com() {
	uint8_t estado;        //variable donde se almacena el estado de la comunicación
	                       //I2C AVR
	estado = TWSR & 0xF8;  //en la variable estado se guarda el valor de los 5 bits de
	                       //mas peso del registro TWSR seguidos de 3 ceros,
						   //el número obtenido indica
						   //el estado en que se encuentra la comunicación I2C AVR

	return estado;         //la función retorna el estado de la comunicación
}
