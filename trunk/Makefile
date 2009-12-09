MCU=atmega16
CC=avr-gcc
OBJCOPY=avr-objcopy
# optimize for size:
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues

all: aquarium.hex
aquarium.hex : aquarium.out 
	$(OBJCOPY) -R .eeprom -O ihex aquarium.out aquarium.hex 
aquarium.out : aquarium.o display.o delays.o thermometer.o eeprom.o locale.o simpledevices.o
	$(CC) $(CFLAGS) -o aquarium.out -Wl,-Map,aquarium.map aquarium.o display.o delays.o thermometer.o eeprom.o locale.o simpledevices.o


aquarium.o : aquarium.c 
	$(CC) $(CFLAGS) -Os -c aquarium.c
display.o : display.c display.h 
	$(CC) $(CFLAGS) -Os -c display.c
delays.o : delays.c delays.h 
	$(CC) $(CFLAGS) -Os -c delays.c
thermometer.o : thermometer.c thermometer.h 
	$(CC) $(CFLAGS) -Os -c thermometer.c
eeprom.o : eeprom.c eeprom.h 
	$(CC) $(CFLAGS) -Os -c eeprom.c
locale.o : locale.c locale.h 
	$(CC) $(CFLAGS) -Os -c locale.c
simpledevices.o : simpledevices.c simpledevices.h
	$(CC) $(CFLAGS) -Os -c simpledevices.c
# erase the AVR before it is programmed
load: aquarium.hex
	uisp -dlpt=/dev/parport0 --erase  -dprog=dapa
	uisp -dlpt=/dev/parport0 --upload if=aquarium.hex -dprog=dapa  -v=3 --hash=32

clean:
	rm -f *.o *.map *.out *.hex
