/*
 * lcd.c
 *
 * Created: 10/27/2021 11:18:50 AM
 * Author : Masauso Lungu
 */ 

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD display when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/

/* Variables ---------------------------------------------------------*/
// Custom character definition using https://omerk.github.io/lcdchargen/
uint8_t customChar[] = {
	
	// addr 0: .....
	0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000,
	// addr 1: |....
	0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000,
	// addr 2: ||...
	0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000,
	// addr 3: |||..
	0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100,
	// addr 4: ||||.
	0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110,
	// addr 5: |||||
	0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111
	
	//// char 1: #
	//0b01010, 0b01010, 0b11111, 0b01010, 0b01010, 0b11111, 0b01010, 0b01010,
	//// char 2: ?
	//0b00000, 0b00000, 0b11111, 0b01010, 0b01010, 0b01010, 0b01010, 0b10011
};

int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);

	// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 48; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);
	
	// Put string(s) at LCD display
	lcd_gotoxy(1, 0);
	lcd_puts("00:00.0");
	
	lcd_gotoxy(11,0);
	lcd_putc('0');

    // Configure 8-bit Timer/Counters for Stopwatch
    // Set the overflow prescalers and enable interrupt
	TIM2_overflow_16ms();
	TIM2_overflow_interrupt_enable();
	
	TIM0_overflow_16ms();
	TIM0_overflow_interrupt_enable();
	
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();

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
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Update the stopwatch on LCD display every sixth overflow,
 *           ie approximately every 100 ms (6 x 16 ms = 100 ms).
 **********************************************************************/
ISR(TIMER2_OVF_vect)
{
	static uint8_t number_of_overflows = 0;
	static uint8_t tens = 0;        // Tenths of a second
	static uint8_t secs = 0;        // Seconds
	
	char lcd_string[2] = "  ";      // String for converting numbers by itoa()
    uint16_t secs_sq = secs*secs;
	

	number_of_overflows++;
	if (number_of_overflows >= 6)
	{
		// Do this every 6 x 16 ms = 100 ms
		number_of_overflows = 0;
	
		// WRITE YOUR CODE HERE
		tens++;
		if (tens > 9)
		{
			tens = 0;
			secs++;
			if (secs > 59)
			{
				secs = 0;
			}
		}
		
		itoa(tens, lcd_string, 10);
		lcd_gotoxy(7, 0);
		lcd_putc(lcd_string[0]);
		
		// Displaying Seconds
		itoa(secs, lcd_string, 10);
		lcd_gotoxy(4, 0);
		if (secs < 10)
		{
			lcd_putc('0');
			lcd_putc(lcd_string[0]);
		}
		else
		{
			lcd_puts(lcd_string);
		}
		
		//Displaying square of seconds
		itoa(secs_sq, lcd_string, 10);
		lcd_gotoxy(11, 0);
		lcd_puts(lcd_string);
		if (secs == 0)
		{
			// Clears the position and reset square of secs back to 0
			secs_sq = 0;
			lcd_puts("       ");
		}

	}
	// Else do nothing and exit the ISR
}

/*--------------------------------------------------------------------*/
/**
 * ISR starts when Timer/Counter0 overflows. Update the progress bar on
 * LCD display every 16 ms.
 */
ISR(TIMER0_OVF_vect)
{
	static uint8_t symbol = 0;
	static uint8_t position = 0;

	symbol++;
	if (symbol >= 8)
	{
		symbol = 0;
		position++;
		if (position > 9)
		{
			position = 0;
			for (uint8_t i = 1; i < 10; i++) // clearing the bar
			{
				lcd_gotoxy(1+i,1);
				lcd_putc(0);
			}
		}
	}
	
	lcd_gotoxy(1 + position, 1);
	lcd_putc(symbol);
	
}

ISR(TIMER1_OVF_vect)
{
	static uint8_t text_display[] = "    I like Digital electronics!\n";
	// text on display
	static uint8_t text_length = sizeof(text_display)/sizeof(char);
	static uint8_t text_position = 0;	// position in the display text
	
	// Displaying moving text
	if (text_position >= text_length)
	{
		text_position = 0;
	}
	lcd_gotoxy(11,1);
	for (uint8_t j = 0; j < 4; j++)
	{
		lcd_gotoxy(11+j,1);
		if (text_position+j >= text_length)
		{
			lcd_putc(text_display[text_position+j-text_length]);
		}
		else lcd_putc(text_display[text_position+j]);
	}
	
	text_position++;
};