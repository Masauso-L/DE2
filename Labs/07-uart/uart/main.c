/*
 * uart.c
 *
 * Created: 11/3/2021 11:14:28 AM
 * Author : masau
 */ 


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library

#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay func
#endif

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion four times 
 *           per second. Send value to LCD and UART.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("value:");
    lcd_gotoxy(3, 1); lcd_puts("key:");
    lcd_gotoxy(8, 0); lcd_puts("a");    // Put ADC value in decimal
    lcd_gotoxy(13,0); lcd_puts("b");    // Put ADC value in hexadecimal
    lcd_gotoxy(8, 1); lcd_puts("c");    // Put button name here


    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
    ADMUX |= (1<< REFS0);
    ADMUX &= ~(1<< REFS1);

    // Set input channel to ADC0
    ADMUX &= ~((1<< MUX0)|(1<< MUX1) | (1<< MUX2) | (1<< MUX3));

    // Enable ADC module
    ADCSRA |= (1<< ADEN);

    // Enable conversion complete interrupt
    ADCSRA |= (1<< ADIE);

    // Set clock prescaler to 128
    ADCSRA |= ((1<< ADPS0)|(1<< ADPS1)|(1<< ADPS2));
   
    // Configure 16-bit Timer/Counter1 to start ADC conversion
    // Set prescaler to 262 ms and enable overflow interrupt
	TIM1_overflow_262ms();
	TIM1_overflow_interrupt_enable();

	// Initialize UART to asynchronous, 8N1, 9600
	uart_init(UART_BAUD_SELECT(9600, F_CPU));

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
 * Purpose:  Use single conversion mode and start conversion four times
 *           per second.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    // Start ADC conversion
	ADCSRA |= (1 << ADSC);
	

}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display value on LCD and send it to UART.
 **********************************************************************/
ISR(ADC_vect)
{
    uint16_t value = 0;
    char lcd_string[8];

    value = ADC;                  // Copy ADC result to 16-bit variable
    itoa(value, lcd_string, 10);  // Convert decimal value to string

    // WRITE YOUR CODE HERE
	lcd_gotoxy(7,0);
	lcd_puts("     ");
	lcd_gotoxy(7,0);
	lcd_puts(lcd_string);
	
	// Send data through UART
	if (value < 700)
	{
		
		uart_puts("ADC value in decimal: ");
		uart_puts(lcd_string);
		uart_puts("\r\n");   // \n means put cursor to next line, \r to the beginning of the current line
	}
	
	// Convert to string in hex
	itoa(value, lcd_string, 16);
	lcd_gotoxy(13,0);
	lcd_puts("     ");
	lcd_gotoxy(13,0);
	lcd_puts(lcd_string);
	
	//Displaying the identity of the button pressed
	if(value >= 1016)
	{
		lcd_gotoxy(7, 1);
		lcd_puts("      ");
		lcd_gotoxy(7, 1);
		lcd_puts("None");
	}
	else if(value == 0)
	{
		lcd_gotoxy(7, 1);
		lcd_puts("     ");
		lcd_gotoxy(7, 1);
		lcd_puts("Right");
	}
	else if(value == 99)
	{
		lcd_gotoxy(7, 1);
		lcd_puts("     ");
		lcd_gotoxy(7, 1);
		lcd_puts("Up");
	}
	else if(value == 256 )
	{
		lcd_gotoxy(7, 1);
		lcd_puts("     ");
		lcd_gotoxy(7, 1);
		lcd_puts("Down");
	}
	else if(value == 409)
	{
		lcd_gotoxy(7, 1);
		lcd_puts("     ");
		lcd_gotoxy(7, 1);
		lcd_puts("Left");
	}
	else if(value == 639)
	{
		lcd_gotoxy(7, 1);
		lcd_puts("      ");
		lcd_gotoxy(7, 1);
		lcd_puts("Select");
	}
	
	//Parity bit evaluation (odd parity)
	if(value %2==0)
	{
		lcd_gotoxy(14,1);
		lcd_putc('1');
	}
	else
	{
		lcd_gotoxy(14,1);
		lcd_putc('0');
	}

}
