/*
 * attiny13_servo_control.c
 * Author : LECLERCQ Antonin
 */ 

#include <avr/io.h>

#define BASE_COUNTER_VALUE 18
#define COUNTER_COEFF 0.08f

void ADC_INIT(void) {
	/*
		PIN PB4 --> ADC2
		Free running mode (continuous conversion)
		Data is left-aligned to get the 8 MSB directly for the timer
	*/
	
	// Connect ADC to channel 2
	ADMUX |= (1 << MUX1);
	ADMUX &= ~(1 << MUX0);
	
	// Left aligned data
	ADMUX |= (1 << ADLAR);
	
	// Set ADC prescaler to 8
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
	ADCSRA &= ~ADPS2;
	
	// Set to free running mode
	ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));
	
	// Enable ADC
	ADCSRA |= (1 << ADEN);
	
	// Enable Auto-triggering
	ADCSRA |= (1 << ADATE);
	
	// Start conversion
	ADCSRA |= (1 << ADSC);
}

void TIMER_INIT(void) {
	/*
		Timer 0 on pin PB0 (OCR0A)
		Fast PWM Mode
		Output frequency --> 73Hz
	*/
	
	// Set Fast PWM Mode
	TCCR0B &= ~(1 << WGM02);
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	
	// Configure Compare Match Output
	TCCR0A |= (1 << COM0A1);
	TCCR0A &= ~(1 << COM0A0);
	
	// Set timer prescaler to 64
	TCCR0B &= ~(1 << CS02);
	TCCR0B |= (1 << CS01) | (1 << CS00);
}

int main(void) {
	
	// Configure ADC
    ADC_INIT();
	
	// Configure Timer 0
	TIMER_INIT();
	
	uint8_t adc_buffer = 0;
	
	// Set PB0 as output
	DDRB |= (1 << PINB0);
	
    while (1) {
		// ADCH register contains the 8 top MSBs
		adc_buffer = (uint8_t)ADCH;
		
		/*
			Timer period = 1/73 = 13.6ms
			Servo motors are controlled by a pulse width between 1ms and 2ms
			--> OCR0A max = (2ms / 13.6ms) * 255 = 38
			--> OCR0A min = (1ms / 13.6ms) * 255 = 18
			--> adc_buffer range = 38 - 18 = 20
			
			--> 20 steps PWM
		*/
		
		// Convert 0..255 to 0..20 for adc_buffer
		adc_buffer = (uint8_t)((float)adc_buffer * COUNTER_COEFF);
		
		// adding OCR0A min value and updating register
		OCR0A = BASE_COUNTER_VALUE + adc_buffer;
	}
	return 0;
}

