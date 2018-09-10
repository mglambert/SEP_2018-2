# Mathías Lambert V.(c) - Lab5 SEP_2018-2

import serial
import time

tarjeta = 'avr'
puerto = 145223 if tarjeta == 'msp' else 14522

# Iniciar conexión serial
micro_controlador = serial.Serial('/dev/cu.usbmodem{}'.format(puerto), 57600, timeout=1)
time.sleep(1.8)
for i in range(ord('a'), ord('z')+1):
    flagCharacter = chr(i).encode()
    micro_controlador.write(flagCharacter)
    rawString = micro_controlador.readline()
    print(rawString.decode()[:-1])

# Cerrar conexión serial
rawString = micro_controlador.readline()
print(rawString.decode())
if tarjeta == 'avr':
    for i in range(7):
        rawString = micro_controlador.readline()
        print(rawString.decode()[:-1])

micro_controlador.close()
