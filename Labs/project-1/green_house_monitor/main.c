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



void ADC_Init()
{
	DDRC=0x0;		/*  Make ADC port as input  */
	ADCSRA = 1 << ADEN | 1 << ADPS0 | 1 << ADPS1 | 1 << ADPS2;	/*  Enable ADC */
}

int ADC_Read()
{
	/* Init ADC to read from internal temperature sensor */
	ADMUX = (1 << MUX3) | (1 << REFS0) | (1 << REFS1);
	
	ADCSRA |= (1<<ADSC);	        /* start conversion  */
	while ((ADCSRA &(1<<ADIF))==0);	/* monitor end of conversion interrupt flag */
	ADCSRA |=(1<<ADIF);	            /* set the ADIF bit of ADCSRA register */
	return(ADCW);		            /* return the ADCW */
}


int main(void)
{
    /* Replace with your application code */
    while (1) 
    {
    }
}

