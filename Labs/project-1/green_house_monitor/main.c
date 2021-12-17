/*
 * green_house_monitor.c
 *
 * Created: 11/26/2021 12:44:01 PM
 * Author : masau
 */ 

#ifndef F_CPU
# define F_CPU 16000000 /**< @brief CPU frequency in Hz required for delay */
#endif

#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include "uart.h"           // library for serial communication
#include "twi.h"            // library for i2c comunication
#include "gpio.h"           // library for manipulating gpio
#include <stdlib.h>         // C library. Needed for conversion function
#include <util/delay.h>
#include <string.h>

#define light_min 35
#define temp_max 35
#define temp_min 22
#define wet_soil 50.00
#define wet_soil 50.00       // Define max value to consider soil 'wet'
#define dry_soil 10.00       // Minimum value for dry soil
#define too_wet_LED PB5      // warning indicator when there is too much water
#define pump     PB4         // Output pin for controlling water pump
#define servo    PD3
#define heater   PB3
#define fan      PB2
#define bulb     PD2

#define SourceVoltage 5000   // Voltage value of the source in mV

float soil_moisture = 0.0;    // soil moisture value in %
uint8_t temp_int, temp_frac;
int lux = 0 ;

int main(void)
{
	// Initialize LCD display
	lcd_init(LCD_DISP_ON);
	lcd_gotoxy(0, 0); lcd_puts("S:");
	lcd_gotoxy(0, 1); lcd_puts("T:");
	lcd_gotoxy(9, 1); lcd_puts("L:");  
	
	
    // Configure ADC to convert PC0[A0] analog value
    // Set ADC reference to AVcc
    ADMUX |= (1 << REFS0);
    ADMUX &= ~(1 << REFS1);
    // Set input channel to ADC0
    ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2) | (1 << MUX3));
    // Enable ADC module
    ADCSRA |= (1 << ADEN);
    // Enable conversion complete interrupt
    ADCSRA |= (1 << ADIE);
    // Set clock prescaler to 128
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	
	// Initialize UART to asynchronous, 8N1, 9600
	uart_init(UART_BAUD_SELECT(9600, F_CPU));
	uart_puts("\r\n---sensor scanner--- \r\n");
	twi_init();
	
	// Configure 16-bit Timer/Counter1
	// Enable interrupt and set the overflow prescaler to 4s
	TIM1_overflow_4s();
	TIM1_overflow_interrupt_enable();
	
	TIM0_overflow_16ms();
	TIM0_overflow_interrupt_enable();
	
	// feedback control elements 
	GPIO_config_output(&DDRB, pump);
	GPIO_config_output(&DDRB, too_wet_LED);
	GPIO_config_output(&DDRB, heater);
	GPIO_config_output(&DDRB, fan);
	GPIO_config_output(&DDRD, bulb);
	GPIO_config_output(&DDRD, servo);
	

	// Enables interrupts by setting the global interrupt mask
	sei();
	
	char uart_string[5];
	
    while (1) 
    {
		/*--------------------------------------------------------------------*/
		/* in this infinite loop, data is sent to the uart and lcd continuously
		 * with a 5s time interval between two sensors. the period for updating 
		 * for individual sensor is 15s
		*/ 
		
		_delay_ms(5000);
        itoa(temp_int, uart_string, 10);
		lcd_gotoxy(3, 1);
		lcd_puts(uart_string);
		lcd_putc('.');
		uart_puts("Temperature(C): ");
        uart_puts(uart_string);
        uart_putc('.');
		
        itoa(temp_frac, uart_string, 10);
		lcd_puts(uart_string);
        uart_puts(uart_string);
		//uart_puts(" \xB0""C.");    // printing degree symbol
        uart_puts("\r\n  \r\n");
		
		_delay_ms(2500);
		dtostrf(soil_moisture,3,2, uart_string); // Convert to string in decimal
		strcat(uart_string,"%");
		uart_puts("Soil Moisture: ");
		uart_puts(uart_string);
		uart_puts("\r\n \r\n");
		lcd_gotoxy(3, 0); lcd_puts(uart_string);
		memset(uart_string,0,10);   // allocating the storage to zero in memory

		_delay_ms(2500);
		itoa(lux, uart_string, 10);
		strcat(uart_string," lx");
		uart_puts("Light intensity: ");
		uart_puts(uart_string);
		uart_puts("\r\n \r\n");
		lcd_gotoxy(12, 1); lcd_puts(uart_string);
    }
}

/************************************************************************/
/* ISR manages the reading of data from two i2c sensors                 */
/************************************************************************/
ISR(TIMER1_OVF_vect)
{
    ;
	/***********************************************
	* temperature sensor is connected via address 0x5c with 
	* resolution of 0x02
	*/
	twi_start((0x5c<<1) + TWI_WRITE);
	twi_write(0x02);
	twi_stop();

	twi_start((0x5c<<1) + TWI_READ);
	temp_int = twi_read_ack();  // reading the integer part
	temp_frac = twi_read_nack(); // reading the decimal part
	twi_stop();
	
	//control feedback for temperature
	if (temp_int < temp_min)
	{
		GPIO_write_high(&PORTB, heater);
		GPIO_write_low(&PORTB, fan);
	}
	
	else if((temp_int >= temp_min) && (temp_int < temp_max))
	{
		//ideal temperature
		GPIO_write_low(&PORTB, heater);
		GPIO_write_low(&PORTB, fan);
	}
	
	else 
	{
		// when the temperature exceed maximum threshold 
		GPIO_write_low(&PORTB, heater);
		GPIO_write_high(&PORTB, fan);
	}

	/***********************************************
	* light sensor is connected via address 0x23 with 
	* resolution of 0x10
	*/
	twi_start((0x23<<1) + TWI_WRITE);
	twi_write(0x10);
	twi_stop();

	twi_start((0x23<<1) + TWI_READ);
	lux = twi_read_ack();
	lux <<=8;
	lux |= twi_read_nack();
	twi_stop();
	lux = lux/1.2;
	
	//control feedback for temperature
	if (lux < light_min)
	{
	GPIO_write_high(&PORTD, bulb);
	}
	else
	{
	//keep lights off when there is sufficient light
	GPIO_write_low(&PORTD, bulb);
	}
	
}


/************************************************************************/
/* ISR manages adc conversion process                                                                     */
/************************************************************************/
ISR(TIMER0_OVF_vect)
{
	// start analogy digital conversion
	ADCSRA |= (1 << ADSC);
	
}

/* -------------------------------------------------------------------*/
/**
 * ISR starts when ADC completes the conversion. Display soil moisture
 * on LCD and send it to UART. high adc value correspond to dry soil and 
 * low value wet soil
 */
ISR(ADC_vect)
{
	//static float dry_soil = 10.00;
	//static float wet_soil = 50.00;
	uint16_t adc_value = 0;
	adc_value = ADCW;    // Copy ADC result to 16-bit variable
	soil_moisture = 100 -(adc_value*100.00)/1023.00;  // soil moisture in %
	

	if (soil_moisture < dry_soil)
	{
		//turn water pump and turn off excess water indicator
		GPIO_write_high(&PORTB, pump);
		GPIO_write_low(&PORTB, too_wet_LED);
	}
	else if ((soil_moisture >= dry_soil) && (soil_moisture < wet_soil))
	{
		//turn off water pump and excess water indicator
		GPIO_write_low(&PORTB,pump);                
		GPIO_write_low(&PORTB, too_wet_LED);      
	}
	
	else if (soil_moisture > wet_soil)
	{
		///turn off water pump and on excess water indicator
		GPIO_write_high(&PORTB, too_wet_LED);
		GPIO_write_low(&PORTB, pump);              
	}
	
	
}

