// Mathi


#define F_CPU	16000000UL

#include <stdlib.h>
#include "PetitFatFilesystem/PetitFS.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "SPI/SPI_implement_me.h"
#include "./libreria/mi_uart.h"
#include "display/ST7735_commands.h"
#include "display/graphic_shapes.h"

void errorHalt(char* msg) {
	enviar_str("Error: ");
	enviar_str(msg);
	enviar_str("\n");
	while(1);
}
/*
void subir_foto(char* filename) {
    int8_t buf[480];	// Buffer for card access
    UINT nr;			// Used for various file access functions.
	if (pf_open(filename)) errorHalt("pf_open");

    if (pf_lseek(54)) errorHalt("pf_lseek");

    for (int i = 0; i < 128; i++) {
        if (pf_read(buf, 480, &nr)) errorHalt("pf_read");
        //if (nr == 0) break;
        for(uint8_t k=0; k<160; k++)
        {
          drawPixelRGB(k, i, buf[2+3*k], buf[1+3*k], buf[0+3*k]);
        }
    }
}
*/

void subir_foto(char* filename) {
    int8_t buf[24];	/* Buffer for card access */
    UINT nr;			/* Used for various file access functions. */
	if (pf_open(filename)) errorHalt("pf_open");

    if (pf_lseek(54)) errorHalt("pf_lseek");

    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 20; j++) {
            if (pf_read(buf, sizeof(buf), &nr)) errorHalt("pf_read");
            if (nr == 0) break;
            for(uint8_t k=0; k<8; k++)
            {
              drawPixelRGB(k+8*j, i, buf[2+3*k], buf[1+3*k], buf[0+3*k]);
            }
        }
    }
}



/* The main function */
int main(void)
{
    DDRD |= (1 << PD6);
	FATFS fs;     		/* File system structure */
    int8_t buf[24];	/* Buffer for card access */
    UINT nr;

    Configuracion* conf = crear_configuracion(57600, 0, 8, 1);
    configurar_uart(conf);
    kill_configuracion(conf);


	// Welcome message
	enviar_str("Lab 12\n\n");

	// Initialise SD and file system.
	//if (pf_mount(&fs)) errorHalt("pf_mount");
    PORTD |= (1 << PD6);
    while (pf_mount(&fs));
    PORTD &= ~(1 << PD6);

    ST7735_init();

    while (1) {
        enviar_str("Seleccione la operacion a realizar:\n");
        enviar_str("\t\t1) SDcard test.\n");
        enviar_str("\t\t2) Mostrar momazo.\n");
        enviar_str("\t\t3) Mostrar galeria de fotos.\n");
        enviar_str("\t\t4) Mostrar animacion.\n");
        enviar_str("\t\t5) Mostrar animacion2.\n");
        enviar_str("\t\t6) Mostrar animacion3.\n\n");

        char s = recibir_char();
        recibir_char();

        switch (s) {
            case '1':
                PORTD |= (1 << PD6);
                while (pf_mount(&fs));

                enviar_str("Hello! This is the SD card test.\n");

                // Initialise SD and file system.
                enviar_str("Trying to mount the SD card's file system: ");
                if (pf_mount(&fs)) errorHalt("pf_mount");
                enviar_str("success.\n");

                // Open read test file.
                enviar_str("Trying to open a text file for reading: ");
                if (pf_open("SEPtest.txt")) errorHalt("pf_open");
                enviar_str("success.\n");

                // Print test file to the serial interface.
                enviar_str("Here is the file content:\n");
                enviar_str("\n-----------------------------------------------\n");
                while (1) {
                  if (pf_read(buf, sizeof(buf), &nr)) errorHalt("pf_read");
                  if (nr == 0) break;
                  for(uint8_t i=0; i<nr; i++)
                  {
                      enviar_caracter(buf[i]);
                  }
                }

                enviar_str("\n-----------------------------------------------\n\n\n");



                // Open write test file.
                enviar_str("Trying to open a text file for writing: ");
                if (pf_open("writeMe.txt")) errorHalt("pf_open");
                enviar_str("success.\n");

                // Set read/write pointer to beginning of file.
                enviar_str("Going to beginning of file: ");
                if (pf_lseek(0)) errorHalt("pf_lseek");
                enviar_str("success.\n");

                // Write data and finish the transaction.
                enviar_str("Writing some data to the file: ");
                char message[] = "If this message does not appear in your text file, you failed to read the PFF documentation. Read about the use of pf_write() there!";
                if (pf_write(message, strlen(message), &nr)) errorHalt("pf_write while writing the file content");
                if (pf_write(0, 0, &nr)) errorHalt("pf_write while closing the file.");
                enviar_str("success.\n");




                // Open write test file.
                enviar_str("Trying to open another text file for writing more: ");
                if (pf_open("writeMe2.txt")) errorHalt("pf_open");
                enviar_str("success.\n");

                // Set read/write pointer to beginning of file.
                enviar_str("Going to beginning of file: ");
                if (pf_lseek(0)) errorHalt("pf_lseek");
                enviar_str("success.\n");

                // Receive a message over USART
                enviar_str("Please type a short message and press enter. ");
                recibir_str(message, strlen(message));
                enviar_str(message);

                // Write it to the file and finish the transaction.
                enviar_str("Writing message to the file: ");
                if (pf_write(message, strlen(message), &nr)) errorHalt("pf_write while writing the file content");
                if (pf_write(0, 0, &nr)) errorHalt("pf_write while closing the file.");
                enviar_str("success.\n");
                enviar_str("\n\n<---- The SD card test is done! ---->\n\n");
                PORTD &= ~(1 << PD6);
                break;

            case '2':
                //if (pf_mount(&fs)) errorHalt("pf_mount");
                PORTD |= (1 << PD6);
                while (pf_mount(&fs));
                subir_foto("momazo.bmp");
                PORTD &= ~(1 << PD6);
                break;

            case '3':
                //if (pf_mount(&fs)) errorHalt("pf_mount");
                PORTD |= (1 << PD6);
                while (pf_mount(&fs));
                subir_foto("foto1.bmp");
                _delay_ms(700);
                subir_foto("foto2.bmp");
                _delay_ms(700);
                subir_foto("foto3.bmp");
                _delay_ms(700);
                subir_foto("flower_r.bmp");
                PORTD &= ~(1 << PD6);
                break;
            case '4':
                fillScreen(ST7735_WHITE);
                int i = TFT_WIDTH/2;
                int j = TFT_HEIGHT/4;
                int i_ans = TFT_WIDTH/2;
                int j_ans = TFT_HEIGHT/4;

                int i1 = TFT_WIDTH/4;
                int j1 = TFT_HEIGHT/2;
                int i1_ans = TFT_WIDTH/4;
                int j1_ans = TFT_HEIGHT/2;

                int i2 = TFT_WIDTH/4;
                int j2 = TFT_HEIGHT/4;
                int i2_ans = TFT_WIDTH/4;
                int j2_ans = TFT_HEIGHT/4;

                int i3 = TFT_WIDTH/3;
                int j3 = TFT_HEIGHT/3;
                int i3_ans = TFT_WIDTH/3;
                int j3_ans = TFT_HEIGHT/3;
                int choques = 0;

                int dirx = 0;
                int diry = 0;
                int dirx1 = 0;
                int diry1 = 0;
                int dirx2 = 0;
                int diry2 = 0;
                int dirx3 = 0;
                int diry3 = 0;

                int init = 0;
                while (choques < 50) {
                    if (i < 5 && dirx == 1) {
                        choques++;
                        dirx = 0;
                    }
                    if (i > 153 && dirx == 0) {
                        choques++;
                        dirx = 1;
                    }
                    if (j < 5 && diry == 1) {
                        choques++;
                        diry = 0;
                    }
                    if (j > 122 && diry == 0) {
                        choques++;
                        diry = 1;
                    }
                    if (init) {
                        fillCircle(i_ans, j_ans, 4, ST7735_WHITE);
                    }
                    fillCircle(i, j, 4, ST7735_BLACK);
                    i_ans = i;
                    j_ans = j;
                    //init = 1;
                    //fillCircle(i, j, 4, ST7735_WHITE);
                    //drawCircle(i, j, 5, ST7735_BLACK);
                    //drawCircle(i, j, 5, ST7735_WHITE);
                    switch (dirx) {
                        case 0:
                            i += rand()%6;
                            break;
                        case 1:
                            i -= rand()%6;
                            break;
                    }
                    switch (diry) {
                        case 0:
                            j += rand()%6;
                            break;
                        case 1:
                            j -= rand()%6;
                            break;
                    }
                    // ball 2
                    if (i1 < 5 && dirx1 == 1) {
                        choques++;
                        dirx1 = 0;
                    }
                    if (i1 > 153 && dirx1 == 0) {
                        choques++;
                        dirx1 = 1;
                    }
                    if (j1 < 5 && diry1 == 1) {
                        choques++;
                        diry1 = 0;
                    }
                    if (j1 > 122 && diry1 == 0) {
                        choques++;
                        diry1 = 1;
                    }
                    if (init) {
                        fillCircle(i1_ans, j1_ans, 4, ST7735_WHITE);
                    }
                    fillCircle(i1, j1, 4, ST7735_RED);
                    i1_ans = i1;
                    j1_ans = j1;
                    //fillCircle(i1, j1, 4, ST7735_WHITE);
                    //drawCircle(i, j, 5, ST7735_BLACK);
                    //drawCircle(i, j, 5, ST7735_WHITE);
                    switch (dirx1) {
                        case 0:
                            i1 += rand()%5;
                            break;
                        case 1:
                            i1 -= rand()%5;
                            break;
                    }
                    switch (diry1) {
                        case 0:
                            j1 += rand()%5;
                            break;
                        case 1:
                            j1 -= rand()%5;
                            break;
                    }

                    // ball 3
                    if (i2 < 5 && dirx2 == 1) {
                        choques++;
                        dirx2 = 0;
                    }
                    if (i2 > 153 && dirx2 == 0) {
                        choques++;
                        dirx2 = 1;
                    }
                    if (j2 < 5 && diry2 == 1) {
                        choques++;
                        diry2 = 0;
                    }
                    if (j2 > 122 && diry2 == 0) {
                        choques++;
                        diry2 = 1;
                    }
                    if (init) {
                        fillCircle(i2_ans, j2_ans, 4, ST7735_WHITE);
                    }
                    fillCircle(i2, j2, 4, ST7735_MAGENTA);
                    i2_ans = i2;
                    j2_ans = j2;
                    //init = 1;
                    //fillCircle(i1, j1, 4, ST7735_WHITE);
                    //drawCircle(i, j, 5, ST7735_BLACK);
                    //drawCircle(i, j, 5, ST7735_WHITE);
                    switch (dirx2) {
                        case 0:
                            i2 += rand()%10;
                            break;
                        case 1:
                            i2 -= rand()%10;
                            break;
                    }
                    switch (diry2) {
                        case 0:
                            j2 += rand()%10;
                            break;
                        case 1:
                            j2 -= rand()%10;
                            break;
                    }
                    // ball 4
                    if (i3 < 5 && dirx3 == 1) {
                        choques++;
                        dirx3 = 0;
                    }
                    if (i3 > 153 && dirx3 == 0) {
                        choques++;
                        dirx3 = 1;
                    }
                    if (j3 < 5 && diry3 == 1) {
                        choques++;
                        diry3 = 0;
                    }
                    if (j3 > 122 && diry3 == 0) {
                        choques++;
                        diry3 = 1;
                    }
                    if (init) {
                        fillCircle(i3_ans, j3_ans, 4, ST7735_WHITE);
                    }
                    fillCircle(i3, j3, 4, ST7735_GREEN);
                    i3_ans = i3;
                    j3_ans = j3;
                    init = 1;
                    //fillCircle(i1, j1, 4, ST7735_WHITE);
                    //drawCircle(i, j, 5, ST7735_BLACK);
                    //drawCircle(i, j, 5, ST7735_WHITE);
                    switch (dirx3) {
                        case 0:
                            i3 += rand()%7;
                            break;
                        case 1:
                            i3 -= rand()%7;
                            break;
                    }
                    switch (diry3) {
                        case 0:
                            j3 += rand()%7;
                            break;
                        case 1:
                            j3 -= rand()%7;
                            break;
                    }
                }
                break;
            case '5':
                fillScreen(ST7735_WHITE);
                int x = TFT_WIDTH/4-10;
                int y = 2*TFT_HEIGHT/4;
                fillRect(x, y, TFT_WIDTH/2+30, TFT_HEIGHT/4, ST7735_RED);
                fillRect(x+2*TFT_WIDTH/8, y-TFT_HEIGHT/6, TFT_WIDTH/4, TFT_HEIGHT/4, ST7735_RED);
                fillTriangle(x+TFT_WIDTH/2, y-TFT_HEIGHT/6, x+TFT_WIDTH/2, y+TFT_HEIGHT/4-1, x+TFT_WIDTH/2+30, y+TFT_HEIGHT/4-1, ST7735_RED);

                fillRect(x+3*TFT_WIDTH/8-2, y-TFT_HEIGHT/6+5, TFT_WIDTH/8, TFT_HEIGHT/8, ST7735_CYAN);
                fillTriangle(x+TFT_WIDTH/2-2, y-TFT_HEIGHT/6+4, x+TFT_WIDTH/2-2, y-TFT_HEIGHT/24+4, x+TFT_WIDTH/2+7, y-TFT_HEIGHT/24+4, ST7735_CYAN);

                fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 12, ST7735_WHITE);
                fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 12, ST7735_WHITE);

                fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);
                fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);

                fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 2, ST7735_WHITE);
                fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 2, ST7735_WHITE);

                drawLine(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4+10, x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4-10, ST7735_WHITE);
                drawLine(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4+10, x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4-10, ST7735_WHITE);
                int y2 = y+TFT_HEIGHT/4+10;

                fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);
                fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);

                int ciclos = 0;

                while (ciclos < 50) {
                    //1
                    //fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);
                    //fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);
                    drawLine(x+(TFT_WIDTH/2+30)/4-8, y+TFT_HEIGHT/4-8, x+(TFT_WIDTH/2+30)/4+8, y+TFT_HEIGHT/4+8, ST7735_BLACK);
                    drawLine(x+3*(TFT_WIDTH/2+30)/4-8, y+TFT_HEIGHT/4-8, x+3*(TFT_WIDTH/2+30)/4+8, y+TFT_HEIGHT/4+8, ST7735_BLACK);

                    fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 2, ST7735_WHITE);
                    fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 2, ST7735_WHITE);

                    drawLine(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4+10, x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4-10, ST7735_WHITE);
                    drawLine(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4+10, x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4-10, ST7735_WHITE);
                    for (volatile int k = 159; k >= 0; k-=8) {
                        drawPixel(k-7, y2, ST7735_BLACK);
                        drawPixel(k-6, y2, ST7735_BLACK);
                        drawPixel(k-5, y2, ST7735_BLACK);
                        drawPixel(k-4, y2, ST7735_BLACK);
                        drawPixel(k-3, y2, ST7735_WHITE);
                        drawPixel(k-2, y2, ST7735_WHITE);
                        drawPixel(k-1, y2, ST7735_WHITE);
                        drawPixel(k, y2, ST7735_WHITE);
                    }
                    //2
                    for (volatile int k = 159; k >= 0; k-=8) {
                        drawPixel(k-7, y2, ST7735_BLACK);
                        drawPixel(k-6, y2, ST7735_BLACK);
                        drawPixel(k-5, y2, ST7735_BLACK);
                        drawPixel(k-4, y2, ST7735_WHITE);
                        drawPixel(k-3, y2, ST7735_WHITE);
                        drawPixel(k-2, y2, ST7735_WHITE);
                        drawPixel(k-1, y2, ST7735_WHITE);
                        drawPixel(k, y2, ST7735_BLACK);
                    }
                    //3
                    //fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);
                    //fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);

                    drawLine(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4+10, x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4-10, ST7735_BLACK);
                    drawLine(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4+10, x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4-10, ST7735_BLACK);

                    fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 2, ST7735_WHITE);
                    fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 2, ST7735_WHITE);

                    drawLine(x+(TFT_WIDTH/2+30)/4-8, y+TFT_HEIGHT/4+8, x+(TFT_WIDTH/2+30)/4+8, y+TFT_HEIGHT/4-8, ST7735_WHITE);
                    drawLine(x+3*(TFT_WIDTH/2+30)/4-8, y+TFT_HEIGHT/4+8, x+3*(TFT_WIDTH/2+30)/4+8, y+TFT_HEIGHT/4-8, ST7735_WHITE);
                    for (volatile int k = 159; k >= 0; k-=8) {
                        drawPixel(k-7, y2, ST7735_BLACK);
                        drawPixel(k-6, y2, ST7735_BLACK);
                        drawPixel(k-5, y2, ST7735_WHITE);
                        drawPixel(k-4, y2, ST7735_WHITE);
                        drawPixel(k-3, y2, ST7735_WHITE);
                        drawPixel(k-2, y2, ST7735_WHITE);
                        drawPixel(k-1, y2, ST7735_BLACK);
                        drawPixel(k, y2, ST7735_BLACK);
                    }
                    //4
                    for (volatile int k = 159; k >= 0; k-=8) {
                        drawPixel(k-7, y2, ST7735_BLACK);
                        drawPixel(k-6, y2, ST7735_WHITE);
                        drawPixel(k-5, y2, ST7735_WHITE);
                        drawPixel(k-4, y2, ST7735_WHITE);
                        drawPixel(k-3, y2, ST7735_WHITE);
                        drawPixel(k-2, y2, ST7735_BLACK);
                        drawPixel(k-1, y2, ST7735_BLACK);
                        drawPixel(k, y2, ST7735_BLACK);
                    }
                    //5
                    //fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);
                    //fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);

                    drawLine(x+(TFT_WIDTH/2+30)/4-8, y+TFT_HEIGHT/4+8, x+(TFT_WIDTH/2+30)/4+8, y+TFT_HEIGHT/4-8, ST7735_BLACK);
                    drawLine(x+3*(TFT_WIDTH/2+30)/4-8, y+TFT_HEIGHT/4+8, x+3*(TFT_WIDTH/2+30)/4+8, y+TFT_HEIGHT/4-8, ST7735_BLACK);

                    fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 2, ST7735_WHITE);
                    fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 2, ST7735_WHITE);

                    drawLine(x+(TFT_WIDTH/2+30)/4-10, y+TFT_HEIGHT/4, x+(TFT_WIDTH/2+30)/4+10, y+TFT_HEIGHT/4, ST7735_WHITE);
                    drawLine(x+3*(TFT_WIDTH/2+30)/4-10, y+TFT_HEIGHT/4, x+3*(TFT_WIDTH/2+30)/4+10, y+TFT_HEIGHT/4, ST7735_WHITE);
                    for (volatile int k = 159; k >= 0; k-=8) {
                        drawPixel(k-7, y2, ST7735_WHITE);
                        drawPixel(k-6, y2, ST7735_WHITE);
                        drawPixel(k-5, y2, ST7735_WHITE);
                        drawPixel(k-4, y2, ST7735_WHITE);
                        drawPixel(k-3, y2, ST7735_BLACK);
                        drawPixel(k-2, y2, ST7735_BLACK);
                        drawPixel(k-1, y2, ST7735_BLACK);
                        drawPixel(k, y2, ST7735_BLACK);
                    }
                    //6
                    for (volatile int k = 159; k >= 0; k-=8) {
                        drawPixel(k-7, y2, ST7735_WHITE);
                        drawPixel(k-6, y2, ST7735_WHITE);
                        drawPixel(k-5, y2, ST7735_WHITE);
                        drawPixel(k-4, y2, ST7735_BLACK);
                        drawPixel(k-3, y2, ST7735_BLACK);
                        drawPixel(k-2, y2, ST7735_BLACK);
                        drawPixel(k-1, y2, ST7735_BLACK);
                        drawPixel(k, y2, ST7735_WHITE);
                    }
                    //7
                    //fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);
                    //fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 10, ST7735_BLACK);

                    drawLine(x+(TFT_WIDTH/2+30)/4-10, y+TFT_HEIGHT/4, x+(TFT_WIDTH/2+30)/4+10, y+TFT_HEIGHT/4, ST7735_BLACK);
                    drawLine(x+3*(TFT_WIDTH/2+30)/4-10, y+TFT_HEIGHT/4, x+3*(TFT_WIDTH/2+30)/4+10, y+TFT_HEIGHT/4, ST7735_BLACK);

                    fillCircle(x+(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 2, ST7735_WHITE);
                    fillCircle(x+3*(TFT_WIDTH/2+30)/4, y+TFT_HEIGHT/4, 2, ST7735_WHITE);

                    drawLine(x+(TFT_WIDTH/2+30)/4-8, y+TFT_HEIGHT/4-8, x+(TFT_WIDTH/2+30)/4+8, y+TFT_HEIGHT/4+8, ST7735_WHITE);
                    drawLine(x+3*(TFT_WIDTH/2+30)/4-8, y+TFT_HEIGHT/4-8, x+3*(TFT_WIDTH/2+30)/4+8, y+TFT_HEIGHT/4+8, ST7735_WHITE);
                    for (volatile int k = 159; k >= 0; k-=8) {
                        drawPixel(k-7, y2, ST7735_WHITE);
                        drawPixel(k-6, y2, ST7735_WHITE);
                        drawPixel(k-5, y2, ST7735_BLACK);
                        drawPixel(k-4, y2, ST7735_BLACK);
                        drawPixel(k-3, y2, ST7735_BLACK);
                        drawPixel(k-2, y2, ST7735_BLACK);
                        drawPixel(k-1, y2, ST7735_WHITE);
                        drawPixel(k, y2, ST7735_WHITE);
                    }
                    // 8
                    for (volatile int k = 159; k >= 0; k-=8) {
                        drawPixel(k-7, y2, ST7735_WHITE);
                        drawPixel(k-6, y2, ST7735_BLACK);
                        drawPixel(k-5, y2, ST7735_BLACK);
                        drawPixel(k-4, y2, ST7735_BLACK);
                        drawPixel(k-3, y2, ST7735_BLACK);
                        drawPixel(k-2, y2, ST7735_WHITE);
                        drawPixel(k-1, y2, ST7735_WHITE);
                        drawPixel(k, y2, ST7735_WHITE);
                    }
                    ciclos++;
                }
                break;
			case '6':
				fillScreen(ST7735_WHITE);
				drawCircle(63, 45, 9, ST7735_BLACK);
				drawPixel(59, 42,ST7735_BLACK);
				drawPixel(60, 42,ST7735_BLACK);
				drawPixel(58, 43,ST7735_BLACK);
				drawPixel(58, 44,ST7735_BLACK);
				drawPixel(59, 45,ST7735_BLACK);
				drawPixel(61, 44,ST7735_BLACK);
				drawPixel(62, 44,ST7735_BLACK);
				drawPixel(65, 43,ST7735_BLACK);
				drawPixel(65, 44,ST7735_BLACK);
				drawPixel(66, 45,ST7735_BLACK);
				drawPixel(67, 45,ST7735_BLACK);
				drawPixel(68, 44,ST7735_BLACK);
				drawPixel(68, 43,ST7735_BLACK);
				drawPixel(67, 42,ST7735_BLACK);
				drawPixel(66, 42,ST7735_BLACK);
				drawPixel(60, 49,ST7735_BLACK);
				drawPixel(61, 50,ST7735_BLACK);
				drawPixel(62, 51,ST7735_BLACK);
				drawPixel(63, 51,ST7735_BLACK);
				drawPixel(64, 51,ST7735_BLACK);
				drawPixel(65, 50,ST7735_BLACK);
				drawPixel(66, 49,ST7735_BLACK);
				drawLine(64, 54, 63, 86, ST7735_BLACK);
				drawLine(63, 86, 53, 110, ST7735_BLACK);
				drawLine(63, 86, 75, 110, ST7735_BLACK);
				drawLine(63, 60, 48, 77, ST7735_BLACK);
				for (volatile int a = 0; a < 50; a++) {
					drawLine(63, 60, 92, 48, ST7735_WHITE);

					drawLine(63, 60, 95, 52, ST7735_BLACK); //1
					_delay_ms(30);
					drawLine(63, 60, 95, 52, ST7735_WHITE);
					drawLine(63, 60, 92, 48, ST7735_BLACK); //2
					_delay_ms(30);
					drawLine(63, 60, 92, 48, ST7735_WHITE);
					drawLine(63, 60, 89, 45, ST7735_BLACK); //3
					_delay_ms(30);

					drawLine(63, 60, 89, 45, ST7735_WHITE);
					drawLine(63, 60, 85, 43, ST7735_BLACK); //4
					_delay_ms(30);

					drawLine(63, 60, 85, 43, ST7735_WHITE);
					drawLine(63, 60, 89, 45, ST7735_BLACK); //3
					_delay_ms(30);

					drawLine(63, 60, 89, 45, ST7735_WHITE);
					drawLine(63, 60, 92, 48, ST7735_BLACK); //2
					_delay_ms(30);
				}
				break;

            default:
                //if (pf_mount(&fs)) errorHalt("pf_mount");
                PORTD |= (1 << PD6);
                while (pf_mount(&fs));
                subir_foto("rocks.bmp");
                PORTD &= ~(1 << PD6);
        }
    }
}
