// Mathías Lambert V. (c) - Lab10 SEP 2018-2

#include "./libreria/mi_uart.h"
#include "./libreria/mi_i2c.h"
#include <stdio.h>
#include "codes.c"

uint16_t in = 0;

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

int main(void) {
    Configuracion* conf = crear_configuracion(57600, 0, 8, 1);
    configurar_uart(conf);
    kill_configuracion(conf);

    init_lab();
    i2c_iniciar();
    while (1){
        i2c_inicia_com();
        i2c_espera();

    	i2c_envia_dato(0xc0);
        char p1 = (char)((housetab[in] & 0xF00) >> 8);
    	i2c_envia_dato(p1);

    	i2c_envia_dato((housetab[in] & 0xFF));
        in++;

        if (in == 512) {
            in = 0;
        }
    	i2c_detener();
    }
}
