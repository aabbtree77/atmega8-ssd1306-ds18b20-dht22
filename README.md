# Non-Blocking Code

This is a custom (soldered) ATmega8 setup with **nonblocking code** to test common sensors DS18B20 and DHT11/22 with a small 128x64 power-economic 
SSD1306 monitor which provides 8 lines each with 21 symbol.

This code might save someone time in finding the correct libs and fitting everything into 5-6KB. Notice that many sensor 
libs use direct datasheet codes which are only preliminary guidelines. They are not suitable for reality yet as they 
are blocking and often deadlocking. **Places with bugs: Check for any code with "while(something)" that does not bail out after 
a fixed number of retries**.

![gThumb](frontside.jpg "Front Side")

![gThumb](backsideside.jpg "Back Side")

# Settings

The fuse bits are set in Makefile such that the clock is internal 1MHz (default), USBasp ISP programmer is employed.

The electric circuit diagram is not provided here, but it is similar to this one:

(https://github.com/protaskin/AVRThermostat)[https://github.com/protaskin/AVRThermostat]

The sensor and monitor wiring is the following:

- SSD1306 - SCL PC5, SDA - PC4 (no freedom (!), this uses an internal I2C circuitry in ATmega8). 
- DS18B20 - PD3. 
- DHT22 - PD2, modify however you like in DHT.h.

If you want to change the DS18B20 port letter, search and replace PORTD, DDRD, PIND in
ds18b20nonblocking.c. The pin number is set in main.c:

```
//DS18b20 is pinned on PD3:
tempS = read_temperature_ds18b20(3);
```

This is useful if multiple DS18B20 sensors are added later without the need of their complex ROM 
address scanning. Pinning several sensors on different port letters is not recommended, but this can be 
done with [C++ and metaprogramming][pin-metaprogramming-C++].

Regarding proper licenses, unfortunately most of the libs have none, but you can safely use at least my parts of code (MIT license), and see how everything is set up. The DS18B20 part is based on a modified old code whose source I can no longer find on github. For massive commercial production one might even need to consider authenticity at the hardware levels, see e.g. [counterfeit_DS18B20]; something similar might be going on with Digispark clones or just about any such device.

# References

- [SSD1306]
- [I2C]
- [DHT22]
- [AVRThermostat]
- [pin-metaprogramming-C++]
-
- [atmega8]

[SSD1306]: https://github.com/Preston-Sundar/AVR-OLED-SSD1306-IIC-DRIVER

[I2C]: https://github.com/Preston-Sundar/AtmegaXX-I2C-Library

[DHT22]: https://github.com/horver/avr_dht22

[AVRThermostat]: https://github.com/protaskin/AVRThermostat

[pin-metaprogramming-C++]: https://github.com/ricardocosme/ds18b20

[counterfeit_DS18B20]: https://github.com/cpetrich/counterfeit_DS18B20

[atmega8]: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2486-8-bit-AVR-microcontroller-ATmega8_L_datasheet.pdf


