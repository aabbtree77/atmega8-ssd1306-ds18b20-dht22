MCU=atmega8
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues
PROG=avrdude
INTERFACE=usbasp

#-------------------
all: main.hex

main.hex : main.out 
	$(OBJCOPY) -R .eeprom -O ihex main.out main.hex 

main.out : main.o SSD1306.o i2c.o ds18b20nonblocking.o DHT.o
	$(CC) $(CFLAGS) -o main.out -Wl,-Map,main.map main.o SSD1306.o i2c.o ds18b20nonblocking.o DHT.o

main.o : main.c SSD1306.h ds18b20nonblocking.h DHT.h
	$(CC) $(CFLAGS) -Os -c main.c
	
SSD1306.o : SSD1306.c SSD1306.h i2c.h
	$(CC) $(CFLAGS) -Os -c SSD1306.c
		
i2c.o : i2c.c i2c.h
	$(CC) $(CFLAGS) -Os -c i2c.c
	
ds18b20nonblocking.o : ds18b20nonblocking.c ds18b20nonblocking.h
	$(CC) $(CFLAGS) -Os -c ds18b20nonblocking.c
		
DHT.o : DHT.c DHT.h
	$(CC) $(CFLAGS) -Os -c DHT.c
#-------------------
help: 
	@echo "Type make or make install"	
#-------------------
install:
	$(PROG) -p $(MCU) -c $(INTERFACE) -U lfuse:w:0xe1:m -U hfuse:w:0xd9:m  -U flash:w:main.hex	
clean:
	rm -f *.o *.map *.out main.hex
#-------------------
