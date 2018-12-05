#include "IRComm.h"

// Defining variables
IRComm::IRComm() {
	initSendTimer();
}


void IRComm::initSendTimer()
{
	// Init timer0 in CTC mode
	TCCR0A |= (1 << WGM01) | (1 << COM0A0);
	// Prescale /256
	TCCR0B |= (1 << CS01);
	OCR0A = this->SENDTOP;

	// Indicated that the timer is properly initiated
	this->sendTimerInited = 1;
}

void IRComm::sendBit(uint8_t sendType)
{
	//if(!sendTimerInited)
		//Serial.println("Error, send bit timer not initiated)
	//else
		// Set which type of bit to send
		sendType -= 0x30;
		this->bitToSend = sendType;
		// Enable timer overflow interrupts
		TIMSK0 = (1 << OCIE0A);
}
