/*
 * leds.c
 *
 * Created: 9/29/2021 11:54:27 AM
 * Author : masau
 */ 

/* Defines -----------------------------------------------------------*/
//#define LED_GREEN   PB5     // AVR pin where green LED is connected
//#define LED_RED     PC0
#define PUSH_BTN    PD5
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
	// Configure the LEDs at port C
	DDRC = DDRC | (0b00011111);
	PORTC = PORTC &~(0b00011111);

    // Configure Push button at port D and enable internal pull-up resistor
	DDRD = DDRD & ~(1<<PUSH_BTN);
	PORTD = PORTD | (1<<PUSH_BTN);
	
	while(1)
	{ 
		//Knight Rider
		_delay_ms(BLINK_DELAY);
		
		while (bit_is_clear(PIND, PUSH_BTN))
		{
			for(int i = 0; i < 5; i++)
			{
				_delay_ms(BLINK_DELAY);
				PORTC = ~((0b00000001) << i); //setting on one led only
			}
			for(int i = 0; i < 5; i++)
			{
				_delay_ms(BLINK_DELAY);
				PORTC = ~((0b00010000) >> i);//LEDs blinking in reverse
			}
		}
	}
 

    // Will never reach this
    return 0;
}
