#include "IRComm.h"

// Defining variables
uint8_t bitType;

// Timer overflow interrupt for sending data
ISR(TIMER0_COMPA_vect)
{
	// Count how many pulses have been sent
	step++;

	// If a start bit needs to be sent...
	if(bitType == START_TYPE)
	{
		/* If (START_BIT/2) have been sent...
		 * The constant variable START_BIT stores the amount of pulses that needs to be sent times 2
		 * A pulse consists of a hill and a pit and for every change an interrupt is generated
		 * So a pulse causes two interrupts
		 */
		if(step > START_BIT)
		{
			// Stop giving timer outputs
			TCCR0A &= ~(1 << COM0A0);
			// Disable the IR LED
			PORTB &= ~(1 << PORTB5);
		}
		/* If more than 80 steps have been counted...
		 * A message ends after 40 counts have been over
		 * The amount of pulses within 40 counts indicates what type of bit was sent
		 */
		if(step > 80)
		{
			TCCR0A |= (1 << COM0A0);
			step = 0;
		}
		/* If the count is lower than the amount of pulses needed to be sent for the bit type...
		 * Switch the IR LED
		 * The amount of times this is done indicates what type of bit is sent
		 */
		if(step < START_BIT)
		{
			PORTB ^= (1 << PORTB5);
		}
	}
	else if(bitType == STOP_TYPE)
	{
		if(step > STOP_BIT)
		{
			TCCR0A &= ~(1 << COM0A0);
			PORTB &= ~(1 << PORTB5);
		}
		if(step > 80)
		{
			TCCR0A |= (1 << COM0A0);
			step = 0;
		}
		if(step < STOP_BIT)
		{
			PORTB ^= (1 << PORTB5);
		}
	else if(bitType == ONE_TYPE)
	{
		if(step > ONE_BIT)
		{
			TCCR0A &= ~(1 << COM0A0);
			PORTB &= ~(1 << PORTB5);
		}
		if(step > 80)
		{
			TCCR0A |= (1 << COM0A0);
			step = 0;
		}
		if(step < ONE_BIT)
		{
			PORTB ^= (1 << PORTB5);
		}
	}
	else if(bitType == ZERO_TYPE)
	{
		if(step > ZERO_BIT)
		{
			TCCR0A &= ~(1 << COM0A0);
			PORTB &= ~(1 << PORTB5);
		}
		if(step > 80)
		{
			TCCR0A |= (1 << COM0A0);
			step = 0;
		}
		if(step < ZERO_BIT)
		{
			PORTB ^= (1 << PORTB5);
		}
	}


}

void initSendTimer(uint8_t type)
{
	bitType = type;
	// Initialize timer0 in PWM mode, non-inverting
	TCCR0A |= (1 << WGM01) | (1 << COM0A0);
	TCCR0B |= (1 << CS01);
	OCR0A = TOP;

	// Enable timer overflow interrupt
	TIMSK0 |= (1 << OCIE0A);
}

ISR(TIMER2_COMPA_vect)
{
	pulseCounter++;
}

void initReceival()
{
	// Initialize timer2
	pulseCounter = 0;

	// Initialize timer2 in CTC mode
	TCCR2A |= (1 << WGM21) | (1 << COM2A0);
	TCCR2B |= (1 << CS20);

	// Reset the timercounter
	TCNT2 = 0;
	// Enable timer2 overflow interrupt
	TIMSK2 |= (1 << TOIE2);

	// Set compare register
	OCR2A = recTimerOverflow;

	// Initialize pin change interrupts
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT20);
}
