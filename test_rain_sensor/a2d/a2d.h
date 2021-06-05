#pragma once

// A2D clock prescaler select
//		*selects how much the CPU clock frequency is divided
//		to create the A2D clock frequency
//		*lower division ratios make conversion go faster
//		*higher division ratios make conversions more accurate
#define ADC_PRESCALE_DIV2		0x00	///< 0x01,0x00 -> CPU clk/2
#define ADC_PRESCALE_DIV4		0x02	///< 0x02 -> CPU clk/4
#define ADC_PRESCALE_DIV8		0x03	///< 0x03 -> CPU clk/8
#define ADC_PRESCALE_DIV16		0x04	///< 0x04 -> CPU clk/16
#define ADC_PRESCALE_DIV32		0x05	///< 0x05 -> CPU clk/32
#define ADC_PRESCALE_DIV64		0x06	///< 0x06 -> CPU clk/64
#define ADC_PRESCALE_DIV128		0x07	///< 0x07 -> CPU clk/128
// default value
#define ADC_PRESCALE			ADC_PRESCALE_DIV64
// do not change the mask value
#define ADC_PRESCALE_MASK		0x07

// A2D voltage reference select
//		*this determines what is used as the
//		full-scale voltage point for A2D conversions
#define ADC_REFERENCE_AREF		0x00	///< 0x00 -> AREF pin, internal VREF turned off
#define ADC_REFERENCE_AVCC		0x01	///< 0x01 -> AVCC pin, internal VREF turned off
#define ADC_REFERENCE_RSVD		0x02	///< 0x02 -> Reserved
#define ADC_REFERENCE_256V		0x03	///< 0x03 -> Internal 2.56V VREF
// default value
#define ADC_REFERENCE			ADC_REFERENCE_AVCC
// do not change the mask value
#define ADC_REFERENCE_MASK		0xC0

// bit mask for A2D channel multiplexer
#define ADC_MUX_MASK			0x1F

//! Initializes the A/D converter.
/// Turns ADC on and prepares it for use.
void a2dInit(void);

//! Turn off A/D converter
void a2dOff(void);

//! Sets the division ratio of the A/D converter clock.
/// This function is automatically called from a2dInit()
/// with a default value.
void a2dSetPrescaler(unsigned char prescale);

//! Configures which voltage reference the A/D converter uses.
/// This function is automatically called from a2dInit()
/// with a default value.
void a2dSetReference(unsigned char ref);

//! Starts a conversion on A/D channel# ch,
/// returns the 10-bit value of the conversion when it is finished.
unsigned short a2dConvert10bit(unsigned char ch);

