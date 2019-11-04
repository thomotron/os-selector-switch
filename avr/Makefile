MCU=attiny85
ISP=usbtiny
CLOCK=8000000
LFUSE=0x62
HFUSE=0xDF
EFUSE=0xFF

all: main

clean:
	rm *.o *.elf *.hex

main: main.c
	# Compile
	avr-gcc -mmcu=$(MCU) -Os -DF_CPU=$(CLOCK)UL -c main.c

	# Link
	avr-gcc -mmcu=$(MCU) -o main.elf main.o

	# HEXify
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

	# Show program size
	avr-size --format=avr --mcu=$(MCU) main.elf

flash: main
	# Flash
	avrdude -p $(MCU) -c $(ISP) -U flash:w:main.hex

fuse:
	# Burn fuses
	avrdude -p $(MCU) -c $(ISP) U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m
