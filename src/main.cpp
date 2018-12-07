#include <Arduino.h>
#include "IRComm.h"

IRComm * irComm;

// Send Timer overflow interrupt
ISR(TIMER0_COMA_vect)
{
	// If the timer has just begun...
	if(irComm->step == 0)
	{
		// Toggle the LED port on every compare interrupt
		TCCR0A |= (1 << COM0A0);
		//irComm->step++;
		Serial.println("Timer started");
	}
	// If the timer has reached the amount of pulses to send...
	if(irComm->step == irComm->typeToSend)
	{
		// Disable the LED port
		TCCR0A &= ~(1 << COM0A0);
		//irComm->step++;
		Serial.println("Pulses disabled");
	}
	irComm->step++;
	// If the end of the message is reached...
	if(irComm->step == 80)
	{
		// Indicate that the message is completed
		irComm->sendComplete = 1;
		irComm->step = 0;
		TIMSK0 = (0 << OCIE0A);
		Serial.println("Timer stopped");
	}

	// If the message is completed
//	if(irComm->sendComplete == 1)
//	{
//		// Reset the timer
//		irComm->step = 0;
//		// Disable compare interrupts
//		TIMSK0 = (0 << OCIE0A);
//	}

	// Add one to the timer counter
//	irComm->step++;
}

int main(void)
{
	// Initialize timers and other stuff
	init();
	// Set the LED pin to output
	DDRD |= (1 << DDD6);
	
	Serial.begin(9600);

	//TIMSK0 = (0 << OCIE0A);
	//Serial.println("Timer interrupts disabled");

	//TCCR0A = (1 << WGM01) | (1 << WGM00);
	//TCCR0B = (1 << CS01) | (1 << WGM02);
	//Serial.println("Timer mode set");

	//OCR0A = irComm->SENDTOP;
	//Serial.println("Timer compare set");
	
	// Initialize the class
	irComm = new IRComm();
	irComm->sendBit(ZERO_BIT);

	// Loop forever
	while(1)
	{
		// Send some bits
		//irComm->sendBit(ZERO_BIT);
		//while(!irComm->sendComplete);
		//irComm->sendBit(ONE_BIT);
		//while(!irComm->sendComplete);
	}
	
	// Never reached
	return(0);
}
