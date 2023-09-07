#include "App.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "timer.h"
#include "LED.h"
#include "button.h"
#include "std_macros.h"

// Initial states
CAR_LED carLED = GREEN; // 0 green 1 yellow	2 red
CAR_LED prevcarLED = YELLOW;
TRAFFIC_MODE mode = NORMAL; //1 normal 0 pedestrian


void app_init(void) {
    //Car LED initialization
    LED_vInit(LED_CAR_PORT, LED_CAR_G_PIN);
	LED_vInit(LED_CAR_PORT, LED_CAR_Y_PIN);
	LED_vInit(LED_CAR_PORT, LED_CAR_R_PIN);

    // Pedestrian LED initialization
    LED_vInit(LED_PED_PORT, LED_PED_G_PIN);
    LED_vInit(LED_PED_PORT, LED_PED_Y_PIN);
    LED_vInit(LED_PED_PORT, LED_PED_R_PIN);

    // Button initialization
    button_init(BUTTON_1_PORT, BUTTON_1_PIN);

	// LCD initialization
	LCD_vInit();
    
	//LCD_vInit();
	timer_init();

    //Enable Global interrupts & setup rising edge detection for button
	sei();
	RISING_EDGE_SETUP();
	SETUP_INT0();
}

void app_run(void) {
	uint8_t i;

    if (mode == NORMAL || carLED == GREEN || carLED == YELLOW) {
        if (mode == PEDESTRIAN)
            carLED = YELLOW;
        
        // Turn off pedestrian leds
        LED_vTurnOff(LED_PED_PORT, LED_PED_G_PIN);
        LED_vTurnOff(LED_PED_PORT, LED_PED_Y_PIN);
        
		LCD_clearscreen();
		LCD_vSend_string("press to cross");

        switch(carLED){
			case GREEN:
				LED_vTurnOn(LED_CAR_PORT, LED_CAR_G_PIN);
				LED_vTurnOff(LED_CAR_PORT, LED_CAR_Y_PIN);
				LED_vTurnOff(LED_CAR_PORT, LED_CAR_R_PIN);
				
				for(i = 0; i < 50; i++){
					timer_delay(68);
					
					// Check if ISR was called
					if(mode == PEDESTRIAN) {
						break;
					}	
				}

				carLED = YELLOW;
				prevcarLED = GREEN;
				break;
				
			case YELLOW:				
				if(mode == PEDESTRIAN){
					if(prevcarLED != RED){
						LED_vTurnOn(LED_PED_PORT,LED_PED_R_PIN);
						//blink both yellow leds
						for(i = 0; i < 5; i++){
							LED_vTurnOn(LED_CAR_PORT,LED_CAR_Y_PIN);
							LED_vTurnOn(LED_PED_PORT,LED_PED_Y_PIN);
							timer_delay(390);
							LED_vTurnOff(LED_CAR_PORT,LED_CAR_Y_PIN);
							LED_vTurnOff(LED_PED_PORT,LED_PED_Y_PIN);
							timer_delay(190);
							LED_vTurnOn(LED_CAR_PORT,LED_CAR_Y_PIN);
							LED_vTurnOn(LED_PED_PORT,LED_PED_Y_PIN);
							timer_delay(390);
						}
					}
					prevcarLED = YELLOW; //to go to Ped lights logic
					carLED = RED;
					LED_vTurnOn(LED_CAR_PORT,LED_CAR_R_PIN);

				} else {
					for(i = 0; i < 5; i++){
						LED_vTurnOn(LED_CAR_PORT,LED_CAR_Y_PIN);
						timer_delay(380);
						LED_vTurnOff(LED_CAR_PORT,LED_CAR_Y_PIN);
						timer_delay(180);
						LED_vTurnOn(LED_CAR_PORT,LED_CAR_Y_PIN);
						timer_delay(380);
						
						//check if ISR was called
						if(mode == PEDESTRIAN) {
							break;
						}
					}
				}

				LED_vTurnOff(LED_CAR_PORT,LED_CAR_Y_PIN);
				LED_vTurnOff(LED_PED_PORT,LED_PED_Y_PIN);

				//Configure variables for correct switching
				if(prevcarLED == GREEN){
					carLED = RED;
					prevcarLED = YELLOW;
				} else if(prevcarLED == RED){
					carLED = GREEN;
					prevcarLED = YELLOW;
				}
				break;

			case RED:
				LCD_clearscreen();
				LCD_vSend_string("cross the road");
				
				LED_vTurnOff(LED_CAR_PORT,LED_CAR_G_PIN);
				LED_vTurnOff(LED_CAR_PORT,LED_CAR_Y_PIN);
				LED_vTurnOn(LED_CAR_PORT,LED_CAR_R_PIN);

				for(i = 0; i < 50; i++){
					timer_delay(68);
					if(mode == PEDESTRIAN) {
						break;
					}
				}

				prevcarLED = RED;
				carLED = YELLOW;
				break;

			default:
				carLED = RED;
				prevcarLED = YELLOW;
				break;
		}
		
	} else {
		LCD_clearscreen();
		LCD_vSend_string("cross the road");
		
		//Configure PED LEDs
		LED_vTurnOn(LED_PED_PORT, LED_PED_G_PIN);
		LED_vTurnOff(LED_PED_PORT, LED_PED_Y_PIN);
		LED_vTurnOff(LED_PED_PORT, LED_PED_R_PIN);

		//Configure CAR LEDs
		LED_vTurnOff(LED_CAR_PORT, LED_CAR_G_PIN);
		LED_vTurnOff(LED_CAR_PORT, LED_CAR_Y_PIN);
		LED_vTurnOn(LED_CAR_PORT, LED_CAR_R_PIN);

		volatile unsigned int ped_counter = 10;
		LCD_clearscreen();
		LCD_vSend_string("Remaining 10 sec");
		
		while(ped_counter > 0)
		{
			timer_delay(1000);
			ped_counter--;
			LCD_movecursor(1,11);
			LCD_vSend_char(' ');
			LCD_vSend_char((ped_counter % 10) + 48);
		}
		
		//make sure car red light is off
		LED_vTurnOff(LED_CAR_PORT, LED_CAR_R_PIN);
		
		LCD_clearscreen();
		LCD_vSend_string("Don't cross");
		
		//blink both yellow while ped green is on
		for(i = 0; i < 5; i++){
			LED_vTurnOn(LED_CAR_PORT, LED_CAR_Y_PIN);
			LED_vTurnOn(LED_PED_PORT, LED_PED_Y_PIN);
			timer_delay(390);
			LED_vTurnOff(LED_CAR_PORT, LED_CAR_Y_PIN);
			LED_vTurnOff(LED_PED_PORT, LED_PED_Y_PIN);
			timer_delay(190);
			LED_vTurnOn(LED_CAR_PORT, LED_CAR_Y_PIN);
			LED_vTurnOn(LED_PED_PORT, LED_PED_Y_PIN);
			timer_delay(390);
		}

		//Turn off yellow LEDs 
		LED_vTurnOff(LED_CAR_PORT, LED_CAR_Y_PIN);
		LED_vTurnOff(LED_PED_PORT, LED_PED_Y_PIN);

		//turn on PED red LED
		LED_vTurnOn(LED_PED_PORT,LED_PED_R_PIN);

		//reset mode
		mode = NORMAL;

		//Configure carLED variables
		carLED = GREEN;
		prevcarLED = YELLOW;
	}
	
}

ISR(__vector_1){
	mode = PEDESTRIAN;
}
