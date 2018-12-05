#include <Arduino.h>
#include "IRComm.h"

IRComm *irComm;

// Timer overflow interrupt for sending data
ISR(TIMER0_COMPA_vect)
{
	if (irComm->bitToSend == 1)
	{
		if (irComm->step == 0)
		{
			TCCR0A |= (1 << COM0A0);
		}
		else if (irComm->step == ONE_BIT)
		{
			TCCR0A &= ~(1 << COM0A0);
		}
		else if (irComm->step == 80)
		{
			irComm->step = 0;
		}
		/*// 1-40
		if(irComm->step < ONE_BIT)
		{
			PORTB ^= (1 << PORTB5);
		}
		// 41-81
		else if (irComm->step > ONE_BIT)
		{
			DISABLE_LEDPIN;
		}*/
	}
	else if (irComm->bitToSend == 0)
	{
		if (irComm->step > ZERO_BIT)
		{
			TCCR0A &= ~(1 << COM0A0);
			DISABLE_LEDPIN;
		}
		if (irComm->step < ZERO_BIT)
		{
			PORTB ^= (1 << PORTB5);
		}
	}

	irComm->step++;

	//	if (irComm->step == 80)
	//	{
	//		PORTB &= ~(1 << PORTB5);
	// Disable the timer
	// Disable timer overflow interrupts
	//TIMSK0 &= ~(1 << OCIE0A);
	// Reset step counter
	//		irComm->step = 0;
	//	}
}

ISR(TIMER2_COMPA_vect)
{
	// Add one to the bit timer counter
	irComm->bitTimerCounter++;

	// If 80 counts are counted...
	if (irComm->bitTimerCounter > 80)
	{
		// If there have been ZERO_BIT pulses...
		if (irComm->pulseCounter == ZERO_BIT)
			// Set the type of received bit to ZERO_TYPE
			irComm->bitType = ZERO_TYPE;
		// If there have been ONE_BIT pulses...
		else if (irComm->pulseCounter == ONE_BIT)
			// Set the type of received bit to ONE_TYPE
			irComm->bitType = ONE_TYPE;

		// Reset the timer
		TCNT2 = 0;
		// Disable interrupts for this timer
		TIMSK2 &= ~(1 << TOIE2);

		irComm->bitTimerRunning = 0;
		irComm->pulseCounter = 0;

		irComm->bitType += 0x30;
		Serial.print("Received: ");
		Serial.println(irComm->bitType);
	}
}

// Pin Change Interrupt for receiving data
/*ISR(PCINT20_vect)
{
	// Count a pulse
	irComm->pulseCounter++;

	// If the timer is not running
	if (irComm->bitTimerRunning != 1)
	{
		// Signal that the timer is started
		irComm->bitTimerRunning = 1;
		// Reset the counter
		irComm->bitTimerCounter = 0;

		// Initialize timer2 in CTC mode
		TCCR2A |= (1 << WGM21) | (1 << COM2A0);
		TCCR2B |= (1 << CS20);

		// Reset the timercounter
		TCNT2 = 0;
		// Enable timer2 overflow interrupt
		TIMSK2 = (1 << TOIE2);

		// Set output compare register
		OCR2A = irComm->RECTOP;
	}
}*/

int main(void)
{
	//init();
	DDRD |= (1 << DDD6);

	PCICR = (1 << PCIE2);
	PCMSK2 = (1 << PCINT20);

	irComm = new IRComm();
	//Serial.begin(9600);
	//irComm->initSendTimer();
	irComm->sendBit(1);

	while (1)
	{
		/*if(Serial.available() > 0)
		{
			irComm->sendBit(Serial.read());
			Serial.print("Verzend: ");
			Serial.println(irComm->bitToSend, HEX);
		}*/
		irComm->sendBit(1);
		//_delay_us(200);
	}

	return (0);
}
