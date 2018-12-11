#include "IRComm.h"
#include <util/delay.h>

// Defining variables
IRComm::IRComm()
{
	// Set timer0 on CTC mode, overflow at TOP, /8 prescaler
	// The frequency is defined in IRComm.h in PWMFREQ
	TCCR0A = (1 << COM0A0) | (1 << WGM00) | (1 << WGM01);
	TCCR0B = (1 << CS00) | (1 << WGM02);
	//Serial.println("T0 init");
	// Set Output Compare Register to TOP
	OCR0A = SENDTOP;
	//Serial.println("OCR0  set");

	// Enable timer overflow interrupts
	TIMSK0 = (1 << OCIE0A);
	//Serial.println("TOI enabled");
}

void IRComm::sendBit(uint8_t sendType)
{
	// A low signal on Digital PIN 5 allows the sending of the 38kHz or 56kHz signal
	// A high signal on Digital PIN 5 blocks the 38kHz or 56kHz signal

	bitSendComplete = 0;
	// Set which type of bit to send
	bitSendType = sendType;
	//Serial.print("Bit to send: ");
	//Serial.println(sendType);
	bitSendCounter = 0;

	//_delay_us(50);

	// Disable the 'blocking' PIN
	PORTD &= ~(1 << PORTD4);
	//PORTD |= (1 << PORTD4);

	//_delay_us(50);
	//Serial.println("Allow LED");
	// Indicate that a bit wants to be sent
	bitSendEnabled = 1;
	//Serial.println("Enabled bit send");

}
