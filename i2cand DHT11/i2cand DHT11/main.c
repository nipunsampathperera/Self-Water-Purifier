/*
 * Demonstration for using I2C devices/sensors such as compass, eeprom, accelerometer etc.
 *
 * Target: Any AVR with hardware I2C (TWI). TWI : Two Wire Interface.
 *
 * Created: on 29 September 2013
 * Author: Vassilis Serasidis
 *
 * Home: http://www.serasidis.gr
 * e-mail: avrsite@yahoo.gr, info@serasidis.gr 
 *
 * Devices was used:
 * - GY26:     I2C digital compass.
 * - LIS302DL: 3-axes I2C/SPI accelerometer.
 * - PCF8574T: I2C-to-parallel converter. Usually used to drive dot matrix LCDs such as 16x2 or 20x4 characters.
 * - DS1307:   I2C Real-Time-Clock. 
 * - 24C32:    I2C 32kbit eeprom (4 kByte x 8 bit = 32 kBits). 
 *
 *
 */
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "i2cmaster.h"
#include "i2c_lcd.h"
#define DHT11_PIN 5
//#include "24cXX.h"
//#define  Trigger_pin	PA0	/* Trigger pin */
//#define LED1 PB0

//void placeEmptyCharacters (char charsNumber[8], unsigned char emptySpacesNumber);

//--------------------------------------------------------------------

//--------------------------------------------------------------------
uint8_t c=0,I_RH,D_RH,I_Temp,D_Temp,CheckSum;

void Request()				/* Microcontroller send start pulse/request */
{
	DDRD |= (1<<DHT11_PIN);
	PORTD &= ~(1<<DHT11_PIN);	/* set to low pin */
	_delay_ms(18);	/* wait for 20ms */
	PORTD |= (1<<DHT11_PIN);	/* set to high pin */
	//_delay_ms(20);	/* wait for 20ms */
}

void Response()				/* receive response from DHT11 */
{
	DDRD &= ~(1<<DHT11_PIN);
	while(PIND & (1<<DHT11_PIN));
	while((PIND & (1<<DHT11_PIN))==0);
	while(PIND & (1<<DHT11_PIN));
	
}

uint8_t Receive_data()			/* receive data */
{
	for (int q=0; q<8; q++)
	{
		while((PIND & (1<<DHT11_PIN)) == 0);  /* check received bit 0 or 1 */
		_delay_us(30);
		if(PIND & (1<<DHT11_PIN))/* if high pulse is greater than 30ms */
		c = (c<<1)|(0x01);	/* then its logic HIGH */
		else			/* otherwise its logic LOW */
		c = (c<<1);
		while(PIND & (1<<DHT11_PIN));
	}
	return c;
}

int main(void)
{
	char data[5];
	lcd_init(LCD_BACKLIGHT_ON);			/* Initialize LCD */
	lcd_clear();			/* Clear LCD */
	lcd_goto_xy(0,0);		/* Enter column and row position */
	
	while(1)
	{	
lcd_puts("self water");
lcd_goto_xy(0,1);
lcd_puts("purify-Team #26");
		/*lcd_goto_xy(0,0);		Enter column and row position 
		lcd_puts("Humidity =");
		_delay_ms(1000);
		lcd_clear();
		_delay_ms(1000);*/
		Request();		/* send start pulse */
		Response();		/* receive response */
		I_RH=Receive_data();	/* store first eight bit in I_RH */
		D_RH=Receive_data();	/* store next eight bit in D_RH */
		I_Temp=Receive_data();	/* store next eight bit in I_Temp */
		D_Temp=Receive_data();	/* store next eight bit in D_Temp */
		CheckSum=Receive_data();/* store next eight bit in CheckSum */
		
		if ((I_RH + D_RH + I_Temp + D_Temp) != CheckSum)
		{
			lcd_goto_xy(0,0);
			lcd_puts("Error");
		}
		
		else
		{
			itoa(I_RH,data,10);
			lcd_goto_xy(11,0);
			lcd_puts(data);
			lcd_puts(".");
			
			itoa(D_RH,data,10);
			lcd_puts(data);
			lcd_puts("%");

			itoa(I_Temp,data,10);
			lcd_goto_xy(7,1);
			lcd_puts(data);
			lcd_puts(".");
			
			itoa(D_Temp,data,10);
			lcd_puts(data);
			//lcd_sendData(0xDF);
			lcd_puts("C ");
			
			/*itoa(CheckSum,data,10);
			lcd_puts(data);
			lcd_puts(" ");*/
			
		}
		_delay_ms(1000);
		lcd_clear();
		_delay_ms(1000);
	}
}
		