#ifndef BUTTON_H_
#define BUTTON_H_

#include "DIO.h"
#include <stdint.h>

//Button port and pin
#define BUTTON_1_PORT 'D' 
#define BUTTON_1_PIN 2


//initialize
void button_init(uint8_t buttonPort,uint8_t buttonPin);

// button read
void button_read(uint8_t buttonPort,uint8_t buttonPin,uint8_t *value);



#endif /* BUTTON_H_ */