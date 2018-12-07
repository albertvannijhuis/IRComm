#include "IRComm.h"

// Defining variables
IRComm::IRComm() {
	initSendTimer();
}


void IRComm::initSendTimer()
{
	// Disable global interrupts
	//cli();
	//Serial.println("Disabled global interrupts");

	// Disable Timer Compare Interrupts
	TIMSK0 = (0 << OCIE0A);
	Serial.println("Timer interrupts disabled");

	// Init timer0 in FastPWM Mode with 8 prescaler
	TCCR0A = (1 << WGM01) | (1 << WGM00);
	TCCR0B = (1 << CS01) | (1 << WGM02);
	Serial.println("Timer mode set");

	// Set Output Compare Register
	OCR0A = this->SENDTOP;
	Serial.println("Timer compare set");

	// Enable global intterupts
	//sei();
	//Serial.println("Global interrupts enabled");

	// Indicate that the timer is properly initiated
	this->sendTimerInited = 1;
}

void IRComm::sendBit(uint8_t sendType)
{
	this->step = 0;
	// Reset the completion status of the timer
	this->sendComplete = 0;
	// Set which type of bit to send
	this->typeToSend = sendType;
	// Enable timer overflow interrupts
	TIMSK0 = (1 << OCIE0A);
}
