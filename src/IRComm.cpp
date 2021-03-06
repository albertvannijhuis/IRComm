#include "IRComm.h"
#include <util/delay.h>
#include <Wire.h>
IRComm::IRComm()
{
	/// Initialize the send timer
	// Set timer2 in Fast PWM mode, overflow at SENDTOP, no prescaler
	// The timer frequency is defined in IRComm.h in PWMFREQ
	TCCR0A = (1 << COM0A0) | (1 << WGM00) | (1 << WGM01);
	TCCR0B = (1 << CS00) | (1 << WGM02);
	// Set Output Compare Register to SENDTOP
	OCR0A = SENDTOP;

	// Enable timer0 overflow interrupts
	TIMSK0 = (1 << OCIE0A);


	/// Initialize the receive timer
	// Set timer2 in Fast PWM mode, overflow at RECTOP, no prescaler
	// The timer frequency is defined in IRComm.h in PWMFREQ
	TCCR2A = (1 << COM2A0) | (1 << WGM20) | (1 << WGM21);
	TCCR0B = (1 << CS20) | (1 << WGM22);
	// Set Output Compare Register to RECTOP
	OCR2A = RECTOP;

	// Enable timer2 overflow interrupts
	TIMSK2 = (1 << OCIE2A);


	// Enable pin change interrupts for PCINT[7:0]
	PCICR = (1 << PCIE1);
	// Enable pin change interrupts on digital PIN 8
	PCMSK1 = (1 << PCINT11);
}

void IRComm::sendBit(uint8_t sendType)
{
	// A low signal on Digital PIN 5 allows the sending of the 38kHz or 56kHz signal
	// A high signal on Digital PIN 5 blocks the 38kHz or 56kHz signal

	// Reset the sendBit counters
	bitSendComplete = 0;
	bitSendCounter = 0;
	// Set which type of bit to send
	bitSendType = sendType;
	//_delay_us(50);

	// Disable the 'blocking' PIN
	PORTD &= ~(1 << PORTD4);
	//PORTD |= (1 << PORTD4);

	//_delay_us(50);
	// Indicate that a bit wants to be sent
	bitSendEnabled = 1;
}

void IRComm::startReceive(){
	bitReceiveChanged = 0;
	bitReceiveStarted = 0;
	bitReceiveCounter = 0;
	bitReceiveComplete = 0;
	bitReceiveEnabled = 1;
}

uint8_t IRComm::handleReceive()
{
	typeReceived = bitReceiveChanged;
	//Wire.beginTransmission(56);
	//Wire.write(255);
	//Wire.endTransmission();
	
	PORTB = typeReceived;
	//PORTB = 1;

	if(typeReceived == ZERO_BIT)
		return ZERO_TYPE;
	else if(typeReceived == ONE_BIT)
		return ONE_TYPE;
	else if(typeReceived == START_BIT)
		return START_TYPE;
	else if(typeReceived == STOP_BIT)
		return STOP_TYPE;
	else
		return 10;
}
