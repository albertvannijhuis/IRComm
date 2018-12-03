#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

// Set frequency in kHz
#define SFREQ 56
#if SFREQ == 38
uint8_t TOP = 25;
uint8_t recTmrOvrflw = 1;
#else
uint8_t TOP = 16;
uint8_t recTmrOvrflw = 0;
#endif

uint8_t step = 0;
uint8_t PCINTpulseCounter = 0;
uint16_t T2ovCounter = 0;
uint8_t pulseTimerOn = false;

void startPulseTimer();

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
		PCINTpulseCounter++;

		// If 5 pulses are counted and the timer is off...
		if(PCINTpulseCounter == 5 && pulseTimerOn == false)
		{
			// Start the timer
			startPulseTimer();
		}
	}
}

// Timer2 Overflow Interrupt
ISR(TIMER2_COMPA_vect)
{
	T2ovCounter++;
}

// Initialize timer for receiving data
void initRecTimer()
{
	T2ovCounter = 0;

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
	PCMSK2 = (1 << PCINT20);
}

void startPulseTimer()
{
	pulseTimerOn = true;

	initRecTimer();
}

int main()
{
	Serial.begin(9600);
	// Make sure to set OC0A pin (PIND6) as output
	DDRD |= (1 << DDD6);
	DDRB |= (1 << DDB5);

	// Initialize timers
	initSendTimer();
	initRecTimer();

	// Enable global interrupts
	sei();

	for(;;)
	{
		Serial.println(recTmrOvrflw);
	}

	// Never reached
	return 0;
}
