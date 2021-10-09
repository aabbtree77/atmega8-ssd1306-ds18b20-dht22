#include "SSD1306.h"
#include "ds18b20nonblocking.h"

#include <avr/io.h>

#ifndef F_CPU
#define F_CPU 1000000
#endif        
#include <util/delay.h>


#define _SET(type,name,bit)          type ## name  |= _BV(bit)    
#define _CLEAR(type,name,bit)        type ## name  &= ~ _BV(bit)        
#define _TOGGLE(type,name,bit)       type ## name  ^= _BV(bit)    
#define _GET(type,name,bit)          ((type ## name >> bit) &  1)
#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ _BV(bit)) ) | ( ( 1 & (unsigned char)value ) << bit )
#define OUTPUT(pin)         _SET(DDR,pin)    
#define INPUT(pin)          _CLEAR(DDR,pin)    
#define HIGH(pin)           _SET(PORT,pin)
#define LOW(pin)            _CLEAR(PORT,pin)    
#define TOGGLE(pin)         _TOGGLE(PORT,pin)    
#define READ(pin)           _GET(PIN,pin)

//Output LED
#define OutX B,0

void display_status(uint16_t tempA, uint8_t sign, uint8_t no_answer, uint16_t tempB, uint16_t hum, uint8_t errB){
    OLED_SetCursor(0, 0);        
    OLED_Printf("DS18B20: %u C", tempA); 
        
    OLED_SetCursor(3, 0);        
    OLED_Printf("Pos: %1u, Err: %1u", sign, no_answer); 

    OLED_SetCursor(5, 0);        
    OLED_Printf("DHT22: %u C", tempB); 
        
    OLED_SetCursor(7, 0);        
    OLED_Printf("Hum: %3u %%, Err: %1u", hum, errB); 

}


int main(void) {

    OLED_Init();  //initialize the OLED
    OLED_Clear(); //clear the display (for good measure)

    OUTPUT(OutX);
    LOW(OutX); 

    uint16_t temp = 1234;
    uint16_t hum = 5678;
    uint8_t error_code = 0;
		
    struct Tstruct tempS = {(uint8_t)0, (int8_t)0, (uint8_t)0};
    				
    while(1) {				    	
        
        error_code = dht_GetTempUtil(&temp, &hum);

        //DS18b20 is pinned on PD3:
	tempS = read_temperature_ds18b20(3);
	display_status((uint16_t)tempS.value, tempS.pos_sign, tempS.no_answer, temp, hum, error_code);
	
	//If we get an answer:
	if (tempS.no_answer == 0) {                
        
            if((tempS.value > 28) && (tempS.pos_sign == 1)) {
                HIGH(OutX); 
            }
    
            if((tempS.value <= 28) && (tempS.pos_sign == 1)) {
                LOW(OutX); 
            }
        }
    }   
}
