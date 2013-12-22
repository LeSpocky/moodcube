PROGRAMMER = avrisp2
#PROGRAMMER = usbtiny
CPU_GCC = attiny24
CPU_DUDE = t24

F_CPU = 8000000

CDEFS = -DF_CPU=$(F_CPU)
CFLAGS = -mmcu=$(CPU_GCC) $(CDEFS) -Wall -Os

.phony: clean

all: moodcube.hex

program: moodcube.hex
	avrdude -c $(PROGRAMMER) -P usb -p $(CPU_DUDE) -U flash:w:moodcube.hex

clean:
	rm *.o *.elf *.hex

moodcube.hex: moodcube.c
	avr-gcc $(CFLAGS) -c moodcube.c -o moodcube.o
	avr-gcc $(CFLAGS) moodcube.o -o moodcube.elf
	avr-objcopy -R .eeprom -O ihex moodcube.elf moodcube.hex
