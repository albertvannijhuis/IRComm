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
	irComm->pulseCounter++;
}

// Pin Change Interrupt for receiving data
ISR(PCINT20_vect)
{
	// If a high signal is received...
	if(RCPIN_HIGH)
	{
		// Count a pulse
		irComm->pulseCounter++;

		// If 5 pulses are counted and the timer is off...
		if(irComm->pulseCounter == 5 && irComm->pulseTimerOn == false)
		{
			// Start the timer
			irComm->startPulseTimer();
		}
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
