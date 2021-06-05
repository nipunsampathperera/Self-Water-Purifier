/*
 * solonoid.c
 *
 * Created: 7/24/2019 11:41:18 PM
 * Author : User
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRC |= (1<<PC0);
	while(1){
		PORTC ^= (1<<PC0);
		_delay_ms(1000);
	}
}

