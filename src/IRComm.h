class IRComm
{
  public:

  protected:

  private:
	// Defining variables

	// IF the frequency of the PWM signal should be 38kHz
	#if PWMFREQ == 38
		const uint8_t IRComm::top = 26;
		uint8_t recTimerOverflow = 0;
	#elif PWMFREQ == 56
		const uint8_t top = 16;
		uint8_t recTimerOverflow = 0;
	#else
	#error Invalid PWM Frequency
	#endif

	// Defining functions
	void initSendTimer();
	void initRecTimer();
	void initReceive();
}

// Defining includes
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

// Defining used pins
#define LED_PIN PORTB6
#define PCI_PIN PORTD4

// Defining macros for the ledpin
#define ENABLE_LEDPIN PORTB |= (1 << LED_PIN)
#define DISABLE_LEDPIN PORTB &= ~(1 << LED_PIN)
#define TOGGLE_LEDPIN PORTB ^= (1 << LED_PIN)

// Defining macros for the Pin Change Interrupt pin
#define RCPIN_HIGH !(PORTD & (1 << PCI_PIN))

// Defining pulse spaces between bits
#define START_BIT 80
#define STOP_BIT 60
#define ONE_BIT 40
#define ZERO_BIT 20

// Set frequency in kHz
#define PWMFREQ 38

// Defining functions
