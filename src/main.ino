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

int main(void)
{
	//init();
	// Enable Global Interrupts

	//Serial.begin(2000000);
	//Serial.println("Begin main");


	// If watchdog did a system reset, we set the pins 4 and 6 to 1
	if (MCUSR & (1 << WDRF))
	{
		DDRD = (1 << DDD6) | (1 << DDD4);
		PORTD = (1 << PORTD4) | (1 << PORTD6);
		while (1) ;
	}
	else
	{
		DDRD |= (1 << DDD6) | (1 << DDD4);
		DDRB |= (1 << DDB5);
		PORTD |= (1 << PORTD4);
		PORTB |= (1 << PORTB5);

		init();

		sei();
		//_delay_us(200);
		// Initialize the class

		_delay_ms(1000);

		irComm = new IRComm();

		//_delay_us(200);
		//irComm->sendBit(ONE_BIT);

		//irComm->sendBit(ONE_BIT);
		// Loop forever
		while (1)
		{
			sendBitAndWait(ONE_BIT);
			/*irComm->sendBit(ONE_BIT);

			while (!(irComm->bitSendComplete))
			{
				PORTB &= ~(1 << PORTB5);
			}
			PORTB |= (1 << PORTB5);*/
			_delay_us(100);
			sendBitAndWait(ZERO_BIT);
			/*irComm->sendBit(ZERO_BIT);

			while (!(irComm->bitSendComplete))
			{
				PORTB &= ~(1 << PORTB5);
			}
			PORTB |= (1 << PORTB5);*/
			_delay_us(100);

			_delay_ms(10);
		}
	}
	// Never reached
	return (0);
}

void sendBitAndWait(uint8_t bit){
			irComm->sendBit(bit);

			while (!(irComm->bitSendComplete))
			{
				PORTB &= ~(1 << PORTB5);
			}
			PORTB |= (1 << PORTB5);
}
