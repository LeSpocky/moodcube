PROGRAMMER = avrisp2
#PROGRAMMER = usbtiny
CPU_GCC = attiny24
CPU_DUDE = t24

F_CPU = 8000000

CDEFS = -DF_CPU=$(F_CPU)
CFLAGS = -mmcu=$(CPU_GCC) $(CDEFS) -Wall -Os

PROGRAM = moodcube

.phony: clean

all: $(PROGRAM).hex

program: $(PROGRAM).hex
	avrdude -c $(PROGRAMMER) -P usb -p $(CPU_DUDE) -U flash:w:$(PROGRAM).hex

clean:
	rm *.o *.elf *.hex

$(PROGRAM).hex: $(PROGRAM).c hsv2rgb.c
	avr-gcc $(CFLAGS) -c $(PROGRAM).c -o $(PROGRAM).o
	avr-gcc $(CFLAGS) -c hsv2rgb.c -o hsv2rgb.o
	avr-gcc $(CFLAGS) $(PROGRAM).o hsv2rgb.o -o $(PROGRAM).elf
	avr-objcopy -R .eeprom -O ihex $(PROGRAM).elf $(PROGRAM).hex
