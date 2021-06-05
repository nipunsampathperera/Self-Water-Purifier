
#include <avr/io.h>
#include <avr/interrupt.h>


#include "a2d.h"


unsigned short a2dConvert10bit(unsigned char ch){
	ADMUX = (ADMUX & ~ADC_MUX_MASK) | (ch & ADC_MUX_MASK);	// set channel
	ADCSR |= 1<< ADIF;						// clear hardware "conversion complete" flag
	ADCSR |= 1<< ADSC;						// start conversion
	while( bit_is_set(ADCSR, ADIF) );		// wait until conversion complete
	return (ADCL | (ADCH<<8));	// r
}
void a2dInit(void)
{
	ADCSR |= 1<<ADEN;				// enable ADC (turn on ADC power)
	ADCSR &= ~(1<<ADFR);			// default to single sample convert mode
	a2dSetPrescaler(ADC_PRESCALE);	// set default prescaler
	a2dSetReference(ADC_REFERENCE);	// set default reference
	ADMUX &= ~(1<<ADLAR);				// set to right-adjusted result

	ADCSR |= 1<< ADIE;				// enable ADC interrupts
	sei();							// turn on interrupts (if not already on)
}


// turn off a2d converter
void a2dOff(void)
{
	ADCSR &= ~(1<< ADIE);				// disable ADC interrupts
	ADCSR &= ~(1<< ADEN);				// disable ADC (turn off ADC power)
}

// configure A2D converter clock division (prescaling)
void a2dSetPrescaler(unsigned char prescale)
{
	ADCSR = ((ADCSR & ~ADC_PRESCALE_MASK) | prescale);
}

// configure A2D converter voltage reference
void a2dSetReference(unsigned char ref)
{
	ADMUX = ((ADMUX & ~ADC_REFERENCE_MASK) | (ref<<6));
}

