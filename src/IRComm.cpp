#include "IRComm.h"

// Defining variables
IRComm::IRComm() {
	initSendTimer();
	initReceival();
}

void IRComm::initSendTimer(uint8_t type)
{
	bitType = type;
	// Initialize timer0 in PWM mode, non-inverting
	TCCR0A |= (1 << WGM01) | (1 << COM0A0);
	TCCR0B |= (1 << CS01);
	OCR0A = TOP;

	// Enable timer overflow interrupt
	TIMSK0 |= (1 << OCIE0A);
}

void IRComm::initReceival()
{
	// Initialize timer2
	pulseCounter = 0;

	// Initialize timer2 in CTC mode
	TCCR2A |= (1 << WGM21) | (1 << COM2A0);
	TCCR2B |= (1 << CS20);

	// Reset the timercounter
	TCNT2 = 0;
	// Enable timer2 overflow interrupt
	TIMSK2 |= (1 << TOIE2);

	// Set compare register
	OCR2A = recTimerOverflow;

	// Initialize pin change interrupts
	PCICR |= (1 << PCIE2);
	PCMSK2 |= (1 << PCINT20);
}
