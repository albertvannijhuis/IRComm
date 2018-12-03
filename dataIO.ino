#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

// Set frequency in kHz
#define SFREQ 38
#if SFREQ == 38
uint8_t TOP = 25;
uint8_t recTmrOvrflw = 0;
#else
uint8_t TOP = 16;
uint8_t recTmrOvrflw = 0;
#endif

uint8_t step = 0;
uint8_t pulseCounter = 0;
uint16_t counter = 0;
uint8_t pulseTimerOn = false;

// Timer Interrupt for sending data
ISR(TIMER0_COMPA_vect)
{
	step++;
	if(step > 40) // 20 pulses sent
	{
		TCCR0A &= ~(1 << COM0A0);
		PORTB &= ~(1 << PORTB5);
	}
	if(step > 80) // 40 pulses sent
	{
		TCCR0A |= (1 << COM0A0);
		step = 0;
	}
	if(step < 40)
	{
		PORTB ^= (1 << PORTB5);
	}
}

// Initialize the timer for sending data
void initSendTimer()
{
	// Init timer0 in PWM mode, non-inverting
	TCCR0A |= (1 << WGM01) | (1 << COM0A0);
	// Prescale /256
	TCCR0B |= (1 << CS01);
	OCR0A = TOP;

	// Enable timer overflow interrupt
	TIMSK0 |= (1 << OCIE0A);
}

// Pin Change Interrupt for receiving data
ISR(PCINT20_vect)
{
	// If a high signal is received...
	if(!(PORTD & (1 << PORTD4)))
	{
		// Count a pulse
		pulseCounter++;

		// If 5 pulses are counted and the timer is off...
		if(pulseCounter == 5 && pulseTimerOn == false)
		{
			// Start the timer
			startPulseTimer();
		}
	}
}

// Timer2 Overflow Interrupt
ISR(TIMER2_COMPA_vect)
{
	counter++;
}

// Initialize timer for receiving data
void initRecTimer()
{
	counter = 0;

	// Initialize timer2 in CTC mode
	TCCR2A |= (1 << WGM21) | (1 << COM2A0);
	TCCR2B |= (1 << CS20);

	// Reset the timercounter
	TCNT2 = 0;
	// Enable timer2 overflow interrupt
	TIMSK2 |= (1 << TOIE2);

	// Set Compare Register
	OCR2A = recTmrOvrflw;
}

// Initialize receiving of data
void initReceive()
{
	//Enable Pin Change Interrupts globally and for PCINT20
	PCICR = (1 << PCIE2);
	PCMSK - (1 << PCINT20);
}

int main()
{
	for(;;)
	{

	}

	// Never reached
	return 0;
}
