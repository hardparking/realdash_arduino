PORT=/dev/ttyACM0
MCU=m32u4
CFLAGS=-g -Wall -mcall-prologues -mmcu=atmega32u4 -Os -I/home/adam/.arduino15/packages/arduino/hardware/samd/1.6.17/libraries/SPI -I/home/adam/.arduino15/packages/arduino/hardware/samd/1.6.17/cores/arduino/ -I/home/adam/.arduino15/packages/arduino/tools/CMSIS-Atmel/1.1.0/CMSIS/Device/ATMEL/
LDFLAGS=-Wl,-gc-sections -Wl,-relax
CC=avr-gcc
TARGET=main
OBJECT_FILES=main.o

USER_LIB_PATH += /home/adam/.arduino15/packages/arduino/hardware/samd/1.6.17/libraries/

ARDUINO_LIBS += SPI



all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj *.hex

%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

program: $(TARGET).hex
	avrdude -p $(MCU) -c avr109 -P $(PORT) -U flash:w:$(TARGET).hex
