/*
    File:       lcdboard.cpp
    Version:    0.1.0
    Date:       Feb. 21, 2013
	License:	GPL v2
    
	atmega128 board code
    
    ****************************************************************************
    Copyright (C) 2013 Radu Motisan  <radu.motisan@gmail.com>

	http://www.pocketmagic.net

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
    ****************************************************************************
 */

#include <avr/io.h>
#include "timeout.h"
#include <util/delay.h>
#include "led.h"
#include "lcd/hd44780.h"
#include "uart/uart.h"
#include "a2d/a2d.h"

// atmega128 has two leds connected to PA0 and PA1
LED led1, led2;

//HD44780 lcd;
HD44780 lcd;

uint16_t adcread(uint8_t i)
{
	ADMUX = i;
	ADCSRA = (1<<ADEN) | (1<<ADIF) | (1<<ADSC)|7;
	loop_until_bit_is_set(ADCSRA, ADIF);
	uint16_t v = ADCL;
	v|= (ADCH<<8);
	return v;
}

int main(void)
{
	
	// init pir sensor
	//pir1.Init(&DDRC, &PINC, 0);pir1.Get();
	_delay_ms(200);
	// RS, E, D4,D5,D6,D7: default: lcd.lcd_init(&PORTD, PD3, &PORTD, PD4, &PORTD, PD5, &PORTD, PD6, &PORTD, PD7, &PORTB, PB0);
	lcd.lcd_init(&PORTD, PD3, &PORTD, PD4, &PORTD, PD5, &PORTD, PD6, &PORTD, PD7, &PORTB, PB0);
_delay_ms(200);
//a2dInit();
	// show logo, wait a little then go with the main loop
	lcd.lcd_cursor_home();
	lcd.lcd_string("pocketmagic.net   ");
	//_delay_ms(500);
	
	int i = 0;
    while(1)
    {
		i++;
		
		lcd.lcd_cursor_home();
		lcd.lcd_string_format("Rain sensor test\nADC:%d    %d",
		//a2dConvert10bit(0),
		adcread(0),
		 i);
		_delay_ms(500);
    }
}