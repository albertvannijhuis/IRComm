//#include <Arduino.h>
#include "IRComm.h"
#include <avr/io.h>
#include <util/delay.h>

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
		if (irComm->bitSendCounter == 80)
		{
		irComm->bitSendComplete = 1;
			irComm->bitSendCounter = 0;
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

ISR(PCINT0_vect)
{
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
	// If watchdog did a system reset, we set the pins 4 and 6 to 1
	if (MCUSR & (1 << WDRF))
	{
		DDRD = (1 << DDD6) | (1 << DDD4);
		PORTD = (1 << PORTD4) | (1 << PORTD6);
		while (1) ;
	}
	else
	{
		DDRD |= (1 << DDD7) | (1 << DDD6) | (1 << DDD4);
		DDRB |= (1 << DDB5) | (1 << DDB4) | (1 << DDB3) | (1 << DDB2) | (1 << DDB1);
		PORTD |= (1 << PORTD4);
		PORTB |= (1 << PORTB5);

		// Initialize timers
		init();

		// Enable global interrupts
		sei();
		//_delay_us(200);

		//_delay_ms(1000);

		// Initialize the class
		irComm = new IRComm();

		//_delay_us(200);
		//irComm->sendBit(ONE_BIT);

		//irComm->sendBit(ONE_BIT);

		sendBitAndWait(ONE_BIT);
		uint8_t bitch;
		// Loop forever
		while (1)
		{
			if(irComm->bitReceiveComplete)
			{
				bitch = irComm->handleReceive(irComm->bitReceiveCounter);
				if(bitch == 0)
					PORTB |= (1 << PORTB1);
				else if(bitch == 1)
					PORTB |= (1 << PORTB2);
				else if(bitch == 2)
					PORTB |= (1 << PORTB3);
				else if(bitch == 3)
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
