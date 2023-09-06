#include "button.h"

//initialize
void button_init(uint8_t buttonPort,uint8_t buttonPin){
	DIO_vsetPINDir(buttonPort, buttonPin, 0);
}

// button read
void button_read(uint8_t buttonPort,uint8_t buttonPin,uint8_t *value){
	*value = DIO_u8read(buttonPort, buttonPin);
}
