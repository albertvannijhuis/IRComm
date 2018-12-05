#include "IRComm.h"

// Defining variables
IRComm::IRComm() {
	initSendTimer();
}


void IRComm::initSendTimer()
{
	cli();
	// Init timer0 in CTC mode
	//TCCR0A |= (1 << WGM01) | (1 << COM0A0);
	TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A0);
	// Prescale /8
	//TCCR0B |= (1 << CS01);
	TCCR0B |= (1 << CS01) | (1 << WGM02);
	OCR0A = this->SENDTOP;
	sei();
	// Indicated that the timer is properly initiated
	this->sendTimerInited = 1;
}

void IRComm::sendBit(uint8_t sendType)
{
	//if(!sendTimerInited)
		//Serial.println("Error, send bit timer not initiated)
	//else
		// Set which type of bit to send
		this->bitToSend = sendType;
		// Enable timer overflow interrupts
		TIMSK0 = (1 << OCIE0A);
}
