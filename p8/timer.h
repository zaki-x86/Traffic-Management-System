
#ifndef TIMER_H_
#define TIMER_H_

void timer_init(void);

/*
	Function Name        : timer_CTC_init_interrupt
	Function Returns     : void
	Function Arguments   : void
	Function Description : initialize the interrupt of the CTC mode of timer0.
*/
void timer_CTC_init_interrupt(void);

/*
	Function Name        : timer_wave_nonPWM
	Function Returns     : void
	Function Arguments   : void
	Function Description : initialize the generation of a wave on OC0 pin in non PWM mode.
*/
void timer_wave_nonPWM(void);


/*
	Function Name        : timer_wave_PWM
	Function Returns     : void
	Function Arguments   : void
	Function Description : initialize the generation of a wave on OC0 pin in fast PWM mode.
*/
void timer_wave_fastPWM(void);

/*
	Function Name        : timer_wave_phasecorrectPWM
	Function Returns     : void
	Function Arguments   : void
	Function Description : initialize the generation of a wave on OC0 pin in phase correct PWM mode.
*/

void timer_wave_phasecorrectPWM(void);

/**
 * 	Function Name        : timer_delay
 *  Function Returns     : void
 * 	Function Arguments   : uint16_t millisec
 * 	Function Description : delay the program for a specific time.
 */
void timer_delay(uint16_t millisec);

#endif /* TIMER_H_ */
