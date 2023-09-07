#include "App.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD.h"
#include "timer.h"
#include "LED.h"
#include "button.h"
#include "std_macros.h"

volatile unsigned char counter1 = 0;
volatile unsigned char counter_green = 10;
volatile unsigned char counter_yellow = 5;
volatile unsigned char counter_red = 7;

#define LED_CAR_PORT 'A'
#define LED_CAR_G_PIN 0
#define LED_CAR_Y_PIN 1
#define LED_CAR_R_PIN 2

#define LED_PED_PORT 'B'
#define LED_PED_G_PIN 0
#define LED_PED_Y_PIN 1
#define LED_PED_R_PIN 2

uint8_t carLED = 0; //0 green 1 yello	w 2 red
uint8_t prevcarLED = 1;
uint8_t normalmode = 1; //1 normal 0 pedestrian

void turn_on_led_car_g();

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
    
	//LCD_vInit();
	timer_init();

    //Enable Global interrupts & setup rising edge detection for button
	sei();
	RISING_EDGE_SETUP();
	SETUP_INT0();
}

void app_run(void) {
    //variable to be used in for loop
	uint8_t i;

    if (normalmode || carLED == 0 || carLED == 1) {
        if (!normalmode)
            carLED = 1;
        
        // Turn off pedestrian leds
        LED_vTurnOff(LED_PED_PORT, LED_PED_G_PIN);
        LED_vTurnOff(LED_PED_PORT, LED_PED_Y_PIN);
        
        switch(carLED){
			//Case GREEN LED
			case 0:
				LED_vTurnOn(LED_CAR_PORT,LED_CAR_G_PIN);
				LED_vTurnOff(LED_CAR_PORT,LED_CAR_Y_PIN);
				
				for(i=0; i < 50; i++){
					timer_delay(68);
					if(!normalmode)	break;//check if ISR was called
				}
				carLED=1;
				prevcarLED=0;
				break;
				
			//Case YELLOW LED 
			case 1:
				//if not normalmode then we need to blink both
				
				if(!normalmode){
					if(prevcarLED!=2){
						LED_vTurnOn(LED_PED_PORT,LED_PED_R_PIN);
						//blink both yellow leds
						for(i=0; i<5; i++){
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
					prevcarLED=1;//to go to Ped lights logic
					carLED=2;
					LED_vTurnOn(LED_CAR_PORT,LED_CAR_R_PIN);
				} else {
					//blink car yellow led
					for(i=0; i < 5; i++){
						LED_vTurnOn(LED_CAR_PORT,LED_CAR_Y_PIN);
						timer_delay(380);
						LED_vTurnOff(LED_CAR_PORT,LED_CAR_Y_PIN);
						timer_delay(180);
						LED_vTurnOn(LED_CAR_PORT,LED_CAR_Y_PIN);
						timer_delay(380);
						if(!normalmode){//check if ISR was called
							prevcarLED=1;
							break;
						}
					}
				}
				LED_vTurnOff(LED_CAR_PORT,LED_CAR_Y_PIN);
				LED_vTurnOff(LED_PED_PORT,LED_PED_Y_PIN);
				//Configure variables for correct switching
				if(prevcarLED==0){
					carLED=2;
					prevcarLED=1;
				}else if(prevcarLED==2){
					carLED=0;
					prevcarLED=1;
				}
				break;
			//Case RED LED
			case 2:
				LED_vTurnOff(LED_CAR_PORT,LED_CAR_G_PIN);
				LED_vTurnOff(LED_CAR_PORT,LED_CAR_Y_PIN);
				LED_vTurnOn(LED_CAR_PORT,LED_CAR_R_PIN);
				for(i=0; i < 50; i++){
					timer_delay(68);
					if(!normalmode)break;
				}
				prevcarLED=2;
				carLED=1;
				break;
			default:
				carLED=2;
				prevcarLED=1;
				break;
		}
		
	}else{
		//Configure PED LEDs
		LED_vTurnOn(LED_PED_PORT,LED_PED_G_PIN);
		LED_vTurnOff(LED_PED_PORT,LED_PED_Y_PIN);
		LED_vTurnOff(LED_PED_PORT,LED_PED_R_PIN);
		
		//Configure CAR LEDs
		LED_vTurnOff(LED_CAR_PORT,LED_CAR_G_PIN);
		LED_vTurnOff(LED_CAR_PORT,LED_CAR_Y_PIN);
		LED_vTurnOn(LED_CAR_PORT,LED_CAR_R_PIN);
		timer_delay(5000);//5 sec delay
		
		//make sure car red light is off
		LED_vTurnOff(LED_CAR_PORT,LED_CAR_R_PIN);
		
		//blink both yellow while ped green is on
		for(i=0;i<5;i++){
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
		//Turn off yellow LEDs 
		LED_vTurnOff(LED_CAR_PORT,LED_CAR_Y_PIN);
		LED_vTurnOff(LED_PED_PORT,LED_PED_Y_PIN);
		//turn on PED red LED
		LED_vTurnOn(LED_PED_PORT,LED_PED_R_PIN);
		//reset normalmode
		normalmode=1;
		//Configure carLED variables
		carLED=0;
		prevcarLED=1;
	}
	
}

ISR(__vector_1){
	normalmode = 0;
}

void turn_on_led_car_g() {
    LCD_clearscreen();
    LCD_vSend_string("Remaining 10 sec");
    LED_vTurnOn(LED_CAR_PORT, LED_CAR_G_PIN);
    
    while(counter_green > 0)
    {
        if(counter1 >= 10)
        {
            counter1 = 0;
            counter_green--;
            LCD_movecursor(1,11);
            LCD_vSend_char(' ');
            LCD_vSend_char((counter_green % 10) + 48);
        }
    }
    
    _delay_ms(500);
    LED_vTurnOff(LED_CAR_PORT, LED_CAR_G_PIN); 
}

void func() {
    counter_green = 10;
    counter_yellow = 5;
    counter_red = 7;
    LCD_clearscreen();
    LCD_vSend_string("Remaining 10 sec");
    LED_vTurnOn(LED_CAR_PORT, LED_CAR_G_PIN);
    
    while(counter_green > 0)
    {
        if(counter1 >= 10)
        {
            counter1 = 0;
            counter_green--;
            LCD_movecursor(1,11);
            LCD_vSend_char(' ');
            LCD_vSend_char((counter_green % 10) + 48);
        }
    }
    
    _delay_ms(500); 
    LED_vTurnOn(LED_CAR_PORT, LED_CAR_Y_PIN);
    LED_vTurnOff(LED_CAR_PORT, LED_CAR_G_PIN);
    LCD_clearscreen();
    LCD_vSend_string("Remaining  5 sec");

    while(counter_yellow > 0)
    {
        if(counter1 >= 10)
        {
            counter1 = 0;
            counter_yellow--;
            LCD_movecursor(1,11);
            LCD_vSend_char(' ');
            LCD_vSend_char((counter_yellow % 10) + 48);
        }
    }
    _delay_ms(500);
    LED_vTurnOn(LED_CAR_PORT, LED_CAR_R_PIN);
    LED_vTurnOff(LED_CAR_PORT, LED_CAR_Y_PIN);
    LCD_clearscreen();
    LCD_vSend_string("Remaining  7 sec");

    while(counter_red > 0)
    {
        if(counter1 >= 10)
        {
            counter1 = 0;
            counter_red--;
            LCD_movecursor(1,11);
            LCD_vSend_char(' ');
            LCD_vSend_char((counter_red % 10) + 48);
        }
    }
    _delay_ms(500);
    LED_vTurnOff(LED_CAR_PORT, LED_CAR_R_PIN);
}