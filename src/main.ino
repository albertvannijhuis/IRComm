//#include <Arduino.h>
#include "IRComm.h"
#include <avr/io.h>
#include <util/delay.h>
#include <Wire.h>
IRComm *irComm;
void sendBitAndWait(uint8_t bit);

// Send Timer overflow interrupt
ISR(TIMER0_COMPA_vect)
{
	// If a bit wants to be sent...
	if (irComm->bitSendEnabled)
	{
		//PORTD |= (1 << PORTD4);
		// If the counter has reached the amount of pulses...
		// ..for the specified bit to be sent...
		if (irComm->bitSendCounter >= irComm->bitSendType)
		{
			// Disable the 'let-throug' pin for the IR LED
			PORTD |= (1 << PORTD4);
			// Indicate that the bit is sent
			//irComm->bitSendComplete = 0;
		}
		// FOR TESTING PURPOSES ONLY

		// Add one to the pulsecounter
		irComm->bitSendCounter++;
		
		// If the bit is completely sent...
		// (a bit is sent over 100 pulses)
		if (irComm->bitSendCounter == 100)
		{
			// Signal that the bit is completely sent
			irComm->bitSendComplete = 1;
			// Reset the counter
			irComm->bitSendCounter = 0;
			// Disable the sending of a bit
			irComm->bitSendEnabled = 0;
		}
	}else{
		//PORTD &= ~(1 << PORTD4);
	}
}

ISR(TIMER2_COMPA_vect)
{
	if(irComm->bitReceiveEnabled)
	{
		if(irComm->bitReceiveChanged)
		{
			irComm->bitReceiveEnabled = 0;
			irComm->bitReceiveStarted = 0;
			irComm->bitReceiveChanged = 0;
			irComm->bitReceiveComplete = 1;	
			irComm->bitReceiveCounter = 20;
		}
		irComm->bitReceiveCounter++;
	}
}

ISR(PCINT1_vect)
{
	PORTD |= (1 << PORTD2);
	if(!irComm->bitReceiveEnabled)
	{
		irComm->bitReceiveCounter = 0;
		irComm->bitReceiveEnabled = 1;
		irComm->bitReceiveStarted = 1;
	}
	else
	{
		irComm->bitReceiveChanged = 1;
	}
}

int main(void)
{
	DDRB |= (1 << DDB1) | (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);
	DDRD |= (1 << DDD2) | (1 << DDD4) | (1 << DDD6) | (1 << DDD7);

	PORTB |= (1 << PORTB5);
	PORTD |= (1 << PORTD4);

	// Initialize timers
	init();

	// Enable global interrupts
	sei();

	// Initialize the class
	irComm = new IRComm();

	sendBitAndWait(ONE_BIT);
	uint8_t recBit;
	// Loop forever
	while (1)
	{
		if(irComm->bitReceiveComplete)
		{
			recBit = irComm->handleReceive(irComm->bitReceiveCounter);
			if(recBit == 0)
				PORTB |= (1 << PORTB1);
			else if(recBit == 1)
				PORTB |= (1 << PORTB2);
			else if(recBit == 2)
				PORTB |= (1 << PORTB3);
			else if(recBit == 3)
				PORTB |= (1 << PORTB4);
			else
				PORTD |= (1 << PORTD7);
		}
		else
		{
			PORTB &= ~(1 << PORTB5);
		}
		//sendBitAndWait(ONE_BIT);

		//irComm->sendBit(ONE_BIT);
		//
		//while (!(irComm->bitSendComplete))
		//{
		//	PORTB &= ~(1 << PORTB5);
		//}
		//PORTB |= (1 << PORTB5);

		//_delay_us(100);
		//sendBitAndWait(ZERO_BIT);

		//irComm->sendBit(ZERO_BIT);
		//
		//while (!(irComm->bitSendComplete))
		//{
		//	PORTB &= ~(1 << PORTB5);
		//}
		//PORTB |= (1 << PORTB5);

		// DELAYS FOR TESTING PURPOSES ONLY
		//_delay_us(100);

		//_delay_ms(10);
	}

	// Never reached
	return (0);
}

// Starts the sendbit function and waits for the bit to be sent completely before moving on
void sendBitAndWait(uint8_t bit){
	// Start sending the bit
	irComm->sendBit(bit);

	// Wait for the bit to be completely sent
	while (!(irComm->bitSendComplete))
	{
		// Disable digital PIN 13 to indicate the bit is still sending
		PORTB &= ~(1 << PORTB5);
	}
	// Enable digital PIN 13 to indicate that te sending is done
	PORTB |= (1 << PORTB5);
}
