/*
 * segment.c
 *
 * Created: 10/20/2021 11:31:16 AM
 *  Author: masau
 */ 

/* Includes ----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000
#endif
#include <util/delay.h>
#include "gpio.h"
#include "segment.h"

/* Variables ---------------------------------------------------------*/
// Active-low digits 0 to 9
uint8_t segment_value[] = {
	// abcdefgDP
	0b00000011,      // Digit 0
	0b10011111,      // Digit 1
	0b00100101,      // Digit 2
	0b00001101,      // Digit 3
	0b10011001,      // Digit 4
	0b01001001,      // Digit 5
	0b01000001,      // Digit 6
	0b00011111,      // Digit 7
	0b00000001,      // Digit 8
	0b00011001,		 // Digit 9
	0b00000010,      // Digit 0.
	0b10011110,      // Digit 1.
	0b00100100,      // Digit 2.
	0b00001100,      // Digit 3.
	0b10011000,      // Digit 4.
	0b01001000,      // Digit 5.
	0b01000000,      // Digit 6.
	0b00011110,      // Digit 7.
	0b00000000,      // Digit 8.
	0b00011000,		 // Digit 9.
	0b11111111,		 // free digit
	0b01111111,		 // Snake 1
	0b00111111,		 // Snake 2
	0b00011111,		 // Snake 3
	0b00001111,		 // Snake 4
	0b10001111,		 // Snake 5
	0b11001111,		 // Snake 6
	0b11101111,		 // Snake 7
	0b11100111,		 // Snake 8
	0b11100011,		 // Snake 9
	0b01100011,		 // Snake 10
	0b01110011,		 // Snake 11
	0b01111011,		 // Snake 12
};

// Active-high position 0 to 3
uint8_t segment_position[] = {
	// p3p2p1p0....
	0b00010000,   // Position 0
	0b00100000,   // Position 1
	0b01000000,   // ...
    0b10000000};


/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: SEG_init()
 * Purpose:  Configure SSD signals LATCH, CLK, and DATA as output.
 * Returns:  none
 **********************************************************************/
void SEG_init(void)
{
    /* Configuration of SSD signals */
    GPIO_config_output(&DDRD, SEG_LATCH);
    GPIO_config_output(&DDRD, SEG_CLK);
    GPIO_config_output(&DDRB, SEG_DATA);
}

/**********************************************************************
 * Function: SEG_update_shift_regs()
 * Purpose:  Display segments at one position of the SSD.
 * Input:    segments - Segments to be displayed (abcdefgDP, active low)
 *           position - Position of the display where the segments are to 
 *                      be displayed (p3 p2 p1 p0 xxxx, active high)
 * Returns:  none
 **********************************************************************/
void SEG_update_shift_regs(uint8_t segments, uint8_t position)
{
    uint8_t bit_number;
	segments = segment_value[segments];     // 0, 1, ..., 9
	position = segment_position[position];  // 0, 1, 2, 3
	
    // Pull LATCH, CLK, and DATA low
	GPIO_write_low(&PORTD, SEG_LATCH);
	GPIO_write_low(&PORTD, SEG_CLK);
	GPIO_write_low(&PORTB, SEG_DATA);

    // Wait 1 us
	_delay_us(1);

    // Loop through the 1st byte (segments)
    // a b c d e f g DP (active low values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
        // Test LSB of "segments" by & (faster) or % (slower) and... 
        // ...output DATA value
		if ((segments & 1) == 0) 
		{
			GPIO_write_low(&PORTB, SEG_DATA);
		}
		else 
		{
			GPIO_write_high(&PORTB, SEG_DATA);
		}

        // Wait 1 us
		_delay_us(1);

        // Pull CLK high
		GPIO_write_high(&PORTD, SEG_CLK);

        // Wait 1 us
		_delay_us(1);

        // Pull CLK low
		GPIO_write_low(&PORTD, SEG_CLK);

        // Shift "segments"
        segments = segments >> 1;
    }

    // Loop through the 2nd byte (position)
    // p3 p2 p1 p0 . . . . (active high values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
        // Test LSB of "position" by & (faster) or % (slower) and... 
        // ...output DATA value
		if ((position & 1) == 0) 
		{
			GPIO_write_low(&PORTB, SEG_DATA);
		}
		else 
		{
			GPIO_write_high(&PORTB, SEG_DATA);
		}

        // Wait 1 us
		_delay_us(1);

        // Pull CLK high
		GPIO_write_high(&PORTD, SEG_CLK);

        // Wait 1 us
		_delay_us(1);

        // Pull CLK low
		GPIO_write_low(&PORTD, SEG_CLK);

        // Shift "position"
        position = position >> 1;
    }

    // Pull LATCH high
	GPIO_write_high(&PORTD, SEG_LATCH);

    // Wait 1 us
	_delay_us(1);

}

/**********************************************************************
 * Function: SEG_clear()
 **********************************************************************/

void SEG_clear(void)
{
	SEG_update_shift_regs(20, 0);
}

/**********************************************************************
 * Function: SEG_clk_2us()
 **********************************************************************/

void SEG_clk_2us()
{
		// Pull CLK high
		GPIO_write_high(&PORTD, SEG_CLK);
		_delay_us(1);

		// Pull CLK low
		GPIO_write_low(&PORTD, SEG_CLK);
		_delay_us(1);
}