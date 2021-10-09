#include "ds18b20nonblocking.h"

uint8_t DS18B20_init(uint8_t pin_number) { 
    
    uint8_t no_answer;	 		
    
    PORTD &= ~(1 << pin_number);
    DDRD |= (1 << pin_number);
    _delay_us(490);
    DDRD &= ~(1 << pin_number);
    _delay_us(68);
    no_answer = (PIND & (1 << pin_number)); //0 - OK, 1 - not OK
    _delay_us(422);
    
    return no_answer;	
}

uint8_t read_18b20(uint8_t pin_number) {

    uint8_t dat = 0;
    for(uint8_t i = 0; i < 8; i++) {		
        DDRD |= (1 << pin_number);
	_delay_us(2);        
	DDRD &= ~(1 << pin_number);
	_delay_us(4);        
	dat = dat >> 1;	    
	if(PIND & (1 << pin_number)) {			
            dat |= 0x80; 
        }
	_delay_us(62);
    }	
    return dat;
}		

void write_18b20(uint8_t dat, uint8_t pin_number) {

    for(uint8_t j = 0;j < 8;j++) {
        DDRD |= (1 << pin_number);
        _delay_us(2);      		
	if(dat & 0x01) {
            DDRD &= ~(1 << pin_number);	
        } else {
        DDRD |= (1 << pin_number);
        }
        dat = dat >> 1; 
        _delay_us(62); 
        DDRD &= ~(1 << pin_number);
        _delay_us(2); 
    }
}
struct Tstruct read_temperature_ds18b20(uint8_t pin_number) {
    
    struct Tstruct tempS = {(uint8_t)0, (int8_t)0, (uint8_t)0};
    uint8_t Temp_L;
    uint8_t Temp_H;
    uint8_t NotOK_flag;
    uint8_t temp_sign;
    int8_t temp_int;
    
    NotOK_flag = DS18B20_init(pin_number);
    write_18b20(0xCC, pin_number);                     // check sensor code
    write_18b20(0x44, pin_number);     		// start temperature conversion
    
    _delay_ms(1000);
  
    NotOK_flag = DS18B20_init(pin_number);        	// initialization DS18B20
    write_18b20(0xCC, pin_number);     		// check sensor code
    write_18b20(0xBE, pin_number);     		// prepare reading
    Temp_L = read_18b20(pin_number);			// read the first 2 bytes of scratchpad
    Temp_H = read_18b20(pin_number); 
	
    temp_sign = 1;                                    // temperature sign flag is 1 (plus)	
    if(Temp_H &(1 << 3)) {    	                       // check the sign bit			
        signed int tmp = 0;
	temp_sign = 0;      			// sign flag is 0 (minus)
	tmp = (Temp_H << 8) | Temp_L;
	tmp = -tmp;
	Temp_L = tmp;
	Temp_H = tmp >> 8; 
    }		
    temp_int = (((Temp_H << 4) & 0x70)|(Temp_L >> 4)); // compute the integer value of the temperature
    
    tempS.no_answer = NotOK_flag;
    tempS.value = temp_int;
    tempS.pos_sign = temp_sign;
    
    return tempS;  
}

