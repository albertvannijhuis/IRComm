#include <Arduino.h>
#include "IRComm.h" 

IRComm *irComm;

// Timer overflow interrupt for sending data
ISR(TIMER0_COMPA_vect)
{
	// Count how many pulses have been sent
	irComm->step++;

	// If a start bit needs to be sent...
	if(irComm->bitType == START_TYPE)
	{
		/* If (START_BIT/2) have been sent...
		 * The constant variable START_BIT stores the amount of pulses that needs to be sent times 2
		 * A pulse consists of a hill and a pit and for every change an interrupt is generated
		 * So a pulse causes two interrupts
		 */
		if(irComm->step > START_BIT)
		{
			// Stop giving timer outputs
			TCCR0A &= ~(1 << COM0A0);
			// Disable the IR LED
			DISABLE_LEDPIN;
		}
		/* If more than 80 steps have been counted...
		 * A message ends after 40 counts have been over
		 * The amount of pulses within 40 counts indicates what type of bit was sent
		 */
		if(irComm->step > 80)
		{
			TCCR0A |= (1 << COM0A0);
			irComm->step = 0;
		}
		/* If the count is lower than the amount of pulses needed to be sent for the bit type...
		 * Switch the IR LED
		 * The amount of times this is done indicates what type of bit is sent
		 */
		if(irComm->step < START_BIT)
		{
			TOGGLE_LEDPIN;
		}
	}
	else if(irComm->bitType == STOP_TYPE)
	{
		if(irComm->step > STOP_BIT)
		{
			TCCR0A &= ~(1 << COM0A0);
			DISABLE_LEDPIN;
		}
		if(irComm->step > 80)
		{
			TCCR0A |= (1 << COM0A0);
			irComm->step = 0;
		}
		if(irComm->step < STOP_BIT)
		{
			TOGGLE_LEDPIN;
		}
	}
	else if(irComm->bitType == ONE_TYPE)
	{
		if(irComm->step > ONE_BIT)
		{
			TCCR0A &= ~(1 << COM0A0);
			DISABLE_LEDPIN;
		}
		if(irComm->step > 80)
		{
			TCCR0A |= (1 << COM0A0);
			irComm->step = 0;
		}
		if(irComm->step < ONE_BIT)
		{
			TOGGLE_LEDPIN;
		}
	}
	else if(irComm->bitType == ZERO_TYPE)
	{
		if(irComm->step > ZERO_BIT)
		{
			TCCR0A &= ~(1 << COM0A0);
			DISABLE_LEDPIN;
		}
		if(irComm->step > 80)
		{
			TCCR0A |= (1 << COM0A0);
			irComm->step = 0;
		}
		if(irComm->step < ZERO_BIT)
		{
			TOGGLE_LEDPIN;
		}
	}
}

ISR(TIMER2_COMPA_vect)
{
	irComm->pulseCounter++;
}
int main(void)
{
	irComm = new IRComm();
	while (0)
	{

	}

	return (0);
}
