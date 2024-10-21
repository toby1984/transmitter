INCLUDE_DIRS = -I/home/tobi/apps/arduino_ide/./hardware/tools/avr/avr/include -Iinclude
LIB_DIR = 

AVRCC_CHIP=atmega328p
AVRDUDE_CHIP = m328p
CPU_FREQ=16000000

CFLAGS = -DMCU=atmega328p

AVRDUDE_DEVICE = /dev/ttyACM0

OBJECTS = main.o bitstream.o debug_led.o error.o transmit.o hw.o

COMPILE = avr-gcc

all: main.hex

.c.o:
	$(COMPILE) -DF_CPU=$(CPU_FREQ) -mmcu=$(AVRCC_CHIP) -Wall -Os -Wno-main $(INCLUDE_DIRS) -c $< -o $@

main.elf: $(OBJECTS)
	$(COMPILE) -DF_CPU=$(CPU_FREQ) -mmcu=$(AVRCC_CHIP) -ffunction-sections -Wl,-gc -Wall -Os -Wno-main $(INCLUDE_DIRS) -o main.elf $(OBJECTS) 

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS)

upload: main.hex
	/usr/bin/avrdude -F -V -c stk500v2 -p $(AVRDUDE_CHIP) -P $(AVRDUDE_DEVICE) -b 115200 -U flash:w:main.hex

readfuses:
	/usr/bin/avrdude -F -V -c stk500v2 -p $(AVRDUDE_CHIP) -P $(AVRDUDE_DEVICE) -b 115200 -U lfuse:r:-:b

# write atmega328p fuses to use 8 Mhz internal oscillator
writefuses:
	/usr/bin/avrdude -F -V -c stk500v2 -p $(AVRDUDE_CHIP) -P $(AVRDUDE_DEVICE) -b 115200 -U lfuse:w:0xff:m -U hfuse:w:0xda:m -U efuse:w:0xfd:m
