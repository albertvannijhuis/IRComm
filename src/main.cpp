#include <Arduino.h>
#include "IRComm.h"
#include <avr/io.h>
#include <util/delay.h>

IRComm * irComm;

// Send Timer overflow interrupt
ISR(TIMER0_COMA_vect)
{
	// If a bit wants to be sent...
	if(irComm->bitSendEnabled)
	{
		// If the counter has reached the amount of pulses...
		// ..for the specified bit to be sent...
		if(irComm->bitSendCounter >= irComm->bitSendType)
		{
			// Disable the 'let-throug' pin for the IR LED
			PORTD |= (1 << PORTD4);
			// Indicate that the bit is sent
			irComm->bitSendComplete = 0;
		}
		// FOR TESTING PURPOSES ONLY
		
		// Add one to the pulsecounter
		irComm->bitSendCounter++;
		if(irComm->bitSendCounter == 80)
		{
			irComm->bitSendComplete = 1;
			irComm->bitSendCounter = 0;
			irComm->bitSendEnabled = 0;
		}
	}
}

int main(void)
{
	//init();
	// Enable Global Interrupts
	//sei();
	
	//Serial.begin(2000000);
	//Serial.println("Begin main");

	DDRD |= (1 << DDD6) | (1 << DDD4);
	PORTD |= (1 << PORTD4);

	//_delay_us(200);
	// Initialize the class
	irComm = new IRComm();

	//_delay_us(200);
	irComm->sendBit(ONE_BIT);

	//irComm->sendBit(ONE_BIT);
	// Loop forever
	while(1)
	{
		//_delay_us(200);
		//Serial.println("Send ONE_BIT");
		irComm->sendBit(ONE_BIT);
		//Serial.println("Starting while");
		while(!(irComm->bitSendComplete)){
			//_delay_ms(1);
		}
		//_delay_us(200);
		//{
			//Serial.println("In while...");
		//}
		//Serial.println("Bit sent!");
	}
	
	// Never reached
	return(0);
}
