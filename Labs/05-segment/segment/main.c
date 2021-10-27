/*
 * segment.c
 *
 * Created: 10/20/2021 11:25:56 AM
 * Author : masau
 */ 

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "segment.h"        // Seven-segment display library for AVR-GCC

volatile uint8_t cnt = 0;

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Display decimal counter values on SSD (Seven-segment 
 *           display) when 16-bit Timer/Counter1 overflows.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Configure SSD signals
    SEG_init();

    // Test of SSD: display number '3' at position 0
   // SEG_update_shift_regs(9, 3);

    // Configure 16-bit Timer/Counter1 for Decimal counter
    // Set the overflow prescaler to 262 ms and enable interrupt
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();
	
	TIM0_overflow_4ms();
	TIM0_overflow_interrupt_enable();


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
 * Purpose:  Increment decimal counter value and display it on SSD.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    // WRITE YOUR CODE HERE

	cnt++;
	if (cnt > 59) 
	{
		cnt = 0;
	}
	//SEG_update_shift_regs(pos, 0);

}

ISR(TIMER0_OVF_vect)
{
	// WRITE YOUR CODE HERE
	static uint8_t pos = 0;
	uint8_t ones;
	uint8_t tens;
	
	tens = cnt /10;
	ones = cnt % 10;
	
	if(pos == 0) SEG_update_shift_regs(ones, 0);
	if(pos == 1) SEG_update_shift_regs(tens, 1);
	if(pos == 2) SEG_update_shift_regs(0, 2);
	if(pos == 3) SEG_update_shift_regs(0, 3);
	
	pos++;
	if(pos == 4) pos = 0;

}
