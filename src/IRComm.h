
// Defining includes
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>

// Defining used pins
#define LED_PIN         PORTB6
#define PCI_PIN         PORTD4

// Defining macros for the ledpin
#define ENABLE_LEDPIN   PORTB |= (1 << LED_PIN)
#define DISABLE_LEDPIN  PORTB &= ~(1 << LED_PIN)
#define TOGGLE_LEDPIN   PORTB ^= (1 << LED_PIN)

// Defining macros for the Pin Change Interrupt pin
#define RCPIN_HIGH     !(PORTD & (1 << PCI_PIN))

// Defining pulse spaces between bits
#define START_BIT      80
#define STOP_BIT       60
#define ONE_BIT        40
#define ZERO_BIT       20

// Defining types of bits
#define START_TYPE		1
#define STOP_TYPE		2
#define ONE_TYPE		3
#define ZERO_TYPE		4

// Defining constants
#define PWMFREQ        38


#ifndef IRCOMM_H
#define IRCOMM_H

class IRComm
{
  public:

  protected:

  private:
	// Defining variables
#if PWMFREQ == 38
	uint8_t TOP = 26;
	uint8_t recTimerOverflow = 0;

#elif PWMFREQ == 56
	uint8_t TOP = 16;
	uint8_t recTimerOverflow = 0;

#else
#error Invalid PWM Frequency
#endif

	uint8_t step;
	uint8_t pulseCounter;
	uint16_t counter;
	uint8_t pulseTimerOn = false;

	// Defining functions
	void initSendTimer(uint8_t);
	void initReceival();
};

#endif
