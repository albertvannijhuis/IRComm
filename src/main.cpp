#include <Arduino.h>
#include "IRComm.h"

IRComm *irComm;

// Timer overflow interrupt for sending data
ISR(TIMER0_COMPA_vect)
{
	irComm->step++;

	if (irComm->byteToSend == 1)
	{
		if (irComm->step > ONE_BIT)
		{
			TCCR0A &= ~(1 << COM0A0);
			DISABLE_LEDPIN;
		}

		if(irComm->step < ONE_BIT)
		{
			PORTB ^= (1 << PORTB5);
		}
	}
	else if (irComm->byteToSend == 0)
	{
		if (irComm->step > ZERO_BIT)
		{
			TCCR0A &= ~(1 << COM0A0);
			DISABLE_LEDPIN;
		}
		if(irComm->step < ZERO_BIT)
		{
			PORTB ^= (1 << PORTB5);
		}
	}

	if (irComm->step > 80)
	{
		TCCR0A |= (1 << COM0A0);
		irComm->step = 0;
	}

}

ISR(TIMER2_COMPA_vect)
{
	// Add one to the bit timer counter
	irComm->bitTimerCounter++;

	// If 80 counts are counted...
	if(irComm->bitTimerCounter > 80)
	{
		// If there have been ZERO_BIT pulses...
		if(pulseCounter == ZERO_BIT)
			// Set the type of received bit to ZERO_TYPE
			bitType = ZERO_TYPE;
		// If there have been ONE_BIT pulses...
		else if(pulseCounter == ONE_BIT)
			// Set the type of received bit to ONE_TYPE
			bitType = ONE_TYPE;

		// Reset the timer
		TCNT2 = 0;
		TIMSK2 &= ~(1 << TOIE2);
	}
}

// Pin Change Interrupt for receiving data
ISR(PCINT20_vect)
{
	// Count a pulse
	irComm->pulseCounter++;

	if(bitTimerRunning == 0)
	{
		bitTimerRunning = 1;
		bitTimerCounter = 0;

		// Initialize timer2 in CTC mode
		TCCR2A |= (1 << WGM21) | (1 << COM2A0);
		TCCR2B |= (1 << CS20);

		// Reset the timercounter
		TCNT2 = 0;
		// Enable timer2 overflow interrupt
		TIMSK2 = (1 << TOIE2);

		// Set output compare register
		OCR2A = RECTOP;
	}
}

int main(void)
{
	irComm = new IRComm();
	Serial.begin(9600);
	while (0)
	{
		irComm->byteToSend = Serial.read();
	}

	return (0);
}
