#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 1000000
#endif
#include <util/delay.h>

struct Tstruct {
    uint8_t no_answer;
    int8_t value;
    uint8_t pos_sign;
};

/* Sensor input to Atmega is on the port D, while pin_number is set main.c.
This is useful if pin changes or there are multiple sensors, but they
need to be on the same port D. If you use some other port, go to ds18b20nonblocking.c
and change all the occurances of PIND, PORTD and DDRD into say PINB, PORTB and DDRB.

If multiple sensors are on different ports, then it's a trouble. You can duplicate
functions into say DS18B20_init_A and DS18B20_init_B and using PINA, ... and PINB, ... 
inside each of them, resp. It's ugly, but simple enough, but better put the sensors 
on the same port and simply use the reading function with different input arguments.

For generic pin management with ROM addresses see

https://github.com/ricardocosme/ds18b20

However, it is a complex modern C++ code.
*/

uint8_t DS18B20_init(uint8_t pin_number);

uint8_t read_18b20(uint8_t pin_number);

void write_18b20(uint8_t dat, uint8_t pin_number);

struct Tstruct read_temperature_ds18b20(uint8_t pin_number);


