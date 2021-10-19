/*
 * gpio.c
 *
 * Created: 10/6/2021 11:11:46 AM
 * Author : masau
 */ 

/***********************************************************************
 * 
 * Alternately toggle 5 LEDs when a push button is pressed in a knight  
 * style. Use functions from GPIO library.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_4   PC4     // AVR pin where green LED is connected
#define LED_3   PC3     // AVR pin where green LED is connected
#define LED_2   PC2     // AVR pin where green LED is connected
#define LED_1   PC1     // AVR pin where green LED is connected
#define LED_0   PC0     // AVR pin where green LED is connected
#define BTN     PD5
#define BLINK_DELAY 500
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include "gpio.h"           // GPIO library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle 5 LEDs when a push button is pressed. Functions 
 *           from user-defined GPIO library is used.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Green LEDs at port C
    GPIO_config_output(&DDRC, LED_0);
    GPIO_write_low(&PORTC, LED_0 );
	
	GPIO_config_output(&DDRC, LED_1);
	GPIO_write_low(&PORTC, LED_1 );
	
	GPIO_config_output(&DDRC, LED_2);
	GPIO_write_low(&PORTC, LED_2 );
	
	GPIO_config_output(&DDRC, LED_3);
	GPIO_write_low(&PORTC, LED_3 );
	
	GPIO_config_output(&DDRC, LED_4);
	GPIO_write_low(&PORTC, LED_4 );

    // Configure Push button at port D and enable internal pull-up resistor
    GPIO_config_input_pullup(&DDRD, BTN);
    

    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        _delay_ms(BLINK_DELAY);

        // WRITE YOUR CODE HERE
		if(!GPIO_read(&PIND, BTN))
		{
			//forward toggle
			GPIO_toggle(&PORTC, LED_0 );
			_delay_ms(BLINK_DELAY);
			GPIO_toggle(&PORTC, LED_0);
			GPIO_toggle(&PORTC, LED_1);
			_delay_ms(BLINK_DELAY);
			GPIO_toggle(&PORTC, LED_1);
			GPIO_toggle(&PORTC, LED_2);
			_delay_ms(BLINK_DELAY);
			GPIO_toggle(&PORTC, LED_2);
			GPIO_toggle(&PORTC, LED_3);
			_delay_ms(BLINK_DELAY);
			GPIO_toggle(&PORTC, LED_3);
			GPIO_toggle(&PORTC, LED_4);
			_delay_ms(BLINK_DELAY);
			
			//backward toggle
			GPIO_toggle(&PORTC, LED_4);
			GPIO_toggle(&PORTC, LED_3);
			_delay_ms(BLINK_DELAY);
			GPIO_toggle(&PORTC, LED_3);
			GPIO_toggle(&PORTC, LED_2);
			_delay_ms(BLINK_DELAY);
			GPIO_toggle(&PORTC, LED_2);
			GPIO_toggle(&PORTC, LED_1);
			_delay_ms(BLINK_DELAY);
			GPIO_toggle(&PORTC, LED_1);
			GPIO_toggle(&PORTC, LED_0);
		}
    }

    // Will never reach this
    return 0;
}