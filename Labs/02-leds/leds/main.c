/*
 * leds.c
 *
 * Created: 9/29/2021 11:54:27 AM
 * Author : masau
 */ 

/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // AVR pin where green LED is connected
#define LED_RED     PC0
#define PUSH_BTN    PD0
#define BLINK_DELAY 100
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/sfr_defs.h>


/* Functions ---------------------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Green LED at port B
    // Set pin as output in Data Direction Register...
    DDRB = DDRB | (1<<LED_GREEN);
    // ...and turn LED off in Data Register
    PORTB = PORTB & ~(1<<LED_GREEN);

	// Configure the second LED at port C
	DDRC = DDRC | (1<<LED_RED);
	PORTC = PORTC | (1<<LED_RED);

    // Configure Push button at port D and enable internal pull-up resistor
	DDRD = DDRD & ~(1<<PUSH_BTN);
	PORTD = PORTD | (1<<PUSH_BTN);
	
	while(1)
	{ 
		
		_delay_ms(BLINK_DELAY);
		
		if (bit_is_clear(PIND, PUSH_BTN))
		{
			PORTB = PORTB ^ (1<<LED_GREEN);
			PORTC = PORTC ^ (1<<LED_RED);
		}
	}
 

    // Will never reach this
    return 0;
}
