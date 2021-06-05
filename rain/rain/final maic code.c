i.........3./*
 * rain.c
 
 
 * Created: 7/24/2019 10:42:25 PM
 * Author : User
 */ 
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <avr/io.h>
#define F_CPU 1000000UL
#include "lcd.h"
#include "i2c_lcd.h"
#include "i2cmaster.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>


#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)

//float read_ph();
void InitADC();
uint16_t ReadADC(uint8_t ch);
void uart_init (void);
void uart_transmit  (uint8_t data);
void uart_print(char *data);
unsigned char uart_recieve (void);

char buffer[16];
char data[10];


int main(void)
{
	  DDRD = 0xf0;
	  DDRD |= (1<<PD1) | (1<<PD3);
	 
	  PORTD |= (1<<PD1);
	  PORTD &= ~(1<<PD3);
	  DDRA =0x00;
	  DDRC = 0xff;
	//DDRD =0b00001000;
	PORTD &= ~(1 << PD0); // Pin n goes low
	PORTA |= (1 << PA1); // Pin n goes high
	lcd_init(LCD_BACKLIGHT_ON);
	Lcd4_Init();
	
	lcd_goto_xy(0,0);
	Lcd4_Set_Cursor(1,0);
	
	lcd_puts("Self Water");
	Lcd4_Write_String("Self Water");
	
	lcd_goto_xy(0,1);
	Lcd4_Set_Cursor(2,0);
	
	lcd_puts("Purifier");
	Lcd4_Write_String("Purifier");
	_delay_ms(3000);
	
	lcd_clear();
	Lcd4_Clear();
	
    while (1) 
    {
		
		if (PIND & (1<<PD0))
		{
 			Lcd4_Set_Cursor(1,0);
 			Lcd4_Write_String("Weather");
 rr			Lcd4_Set_Cursor(2,0);
 			Lcd4_Write_String("Dry");
			
			PORTD |= (1<<PD1);
		}
		else
		{
			
			Lcd4_Set_Cursor(1,0);
 			Lcd4_Write_String("Weather");
 			Lcd4_Set_Cursor(2,0);
 			Lcd4_Write_String("Wet");
			
			PORTD &= ~(1<<PD1);
		}
		
		if(PINA & (1<<PA7)){
			lcd_goto_xy(0,0);
			lcd_puts("Water filled ");
			_delay_ms(1000);
			lcd_goto_xy(0,1);
			lcd_puts("Closing Valve");
			PORTD |= (1<<PD3);
			_delay_ms(2000);
			
			lcd_clear();
			lcd_puts("Checking pH");
			InitADC();
			
			lcd_goto_xy(0,1);
			for(int i=0;i<10;i++){
				lcd_puts_at(".",i,1);
				_delay_ms(1000);
			}
// 			float avgPh=sum/10;
			//float a=read_ph();
			float ph;
			uint16_t temp;
			temp = ReadADC(0);
			ph = (14-(temp*14.0/1024));
			ph = ph - pow((7-ph),2)*0.6;
			
			lcd_clear();
			lcd_goto_xy(0,0);
			lcd_puts("pH value:");
			lcd_goto_xy(0,1);
			sprintf(buffer,"%.2f",ph);
			lcd_puts(buffer);
			_delay_ms(3000);
			lcd_clear();
			
			
		}else{
			lcd_goto_xy(0,0);
			lcd_puts("Water filling");
			PORTD &= ~(1<<PD3);
		}
		
    }
}


void InitADC() //needed ---------------------------------
{
	ADMUX=(1<<REFS0);                         // For Aref=AVcc;
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); //Rrescalar div factor =128
}

uint16_t ReadADC(uint8_t ch) //needed---------------------------
{
	//Select ADC Channel ch must be 0-7
	ch=ch&0b00000111;
	ADMUX|=ch;

	//Start Single conversion
	ADCSRA|=(1<<ADSC);

	//Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	//Clear ADIF by writing one to it
	//Note you may be wondering why we have write one to clear it
	//This is standard way of clearing bits in io as said in datasheets.
	//The code writes '1' but it result in setting bit to '0' !!!

	ADCSRA|=(1<<ADIF);

	return(ADC);
}

void uart_init (void)
{
	UBRRH=(BAUDRATE>>8);
	UBRRL=BAUDRATE;
	UCSRB|=(1<<TXEN)|(1<<RXEN);
	UCSRC|=(1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);
}

void uart_transmit  (uint8_t data)
{
	while (!( UCSRA & (1<<UDRE)));
	UDR = data;
}

void uart_print(char *data){
	for(int i=0;i<strlen(data);i++){
		uart_transmit(data[i]);
	}
}

unsigned char uart_recieve (void)
{
	while(!(UCSRA) & (1<<RXC));
	return UDR;
}