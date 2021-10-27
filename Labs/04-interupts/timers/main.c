/*
 * timers.c
 *
 * Created: 10/13/2021 11:59:51 AM
 * Author : masau
 */ 

/* Defines -----------------------------------------------------------*/
#define LED_D1  PB5
#define LED_D2  PB4
#define LED_D3  PB3
#define LED_D4  PB2

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle one LED on the Multi-function shield using 
             the internal 8- or 16-bit Timer/Counter.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Configuration of LED(s) at port B
    GPIO_config_output(&DDRB, LED_D1);
    GPIO_write_low(&PORTB, LED_D1);
	
	GPIO_config_output(&DDRB, LED_D2);
	GPIO_write_low(&PORTB, LED_D2);
	
	GPIO_config_output(&DDRB, LED_D3);
	GPIO_write_low(&PORTB, LED_D3);
	
	//GPIO_config_output(&DDRB, LED_D4);
	//GPIO_write_low(&PORTB, LED_D4);

    // Configuration of 16-bit Timer/Counter1 for LED blinking
    // Set the overflow prescaler to 262 ms and enable interrupt
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();
	
	TIM0_overflow_16ms();
	TIM0_overflow_interrupt_enable();
	
	TIM2_overflow_4ms();
	TIM2_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
		
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Toggle D1 LED on Multi-function shield.
 **********************************************************************/
ISR(TIMER0_OVF_vect)
{

    // WRITE YOUR CODE HERE
	GPIO_toggle(&PORTB, LED_D1);
 
}

ISR(TIMER1_OVF_vect)
{

	// WRITE YOUR CODE HERE
	GPIO_toggle(&PORTB, LED_D2);

}

ISR(TIMER2_OVF_vect)
{

	// WRITE YOUR CODE HERE
	GPIO_toggle(&PORTB, LED_D3);

}

//ISR(TIMER1_OVF_vect)
//{
//
	//// WRITE YOUR CODE HERE
	//GPIO_toggle(&PORTB, LED_D4);
//
//}
