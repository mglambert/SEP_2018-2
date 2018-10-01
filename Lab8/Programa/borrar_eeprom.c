// Mathías Lambert V. (c) - Lab8 SEP 2018-2

#include <avr/eeprom.h>
#include <stdint.h>
#define F_CPU 16000000UL

int main(void) {
    for (int i = 0; i < 1000; i++) {
        eeprom_write_byte((uint8_t*)i, 0);
    }
}
