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

// Defining constants
#define TOP_38KHZ      26 
#define TOP_56KHZ      16
#define PWMFREQ        38


#ifndef IRCOMM_H
#define IRCOMM_H

class IRComm
{
    public:
    // Defining variables
    #if PWMFREQ == 38
    uint8_t top = TOP_38KHZ;
    uint8_t recTimerOverflow = 0;

    #elif PWMFREQ == 56
    uint8_t top = TOP_56KHZ;
    uint8_t recTimerOverflow = 0;
    
    #else
    #error Invalid PWM Frequency
    #endif

    uint8_t step;
    uint8_t pulseCounter;
    uint16_t counter;
    uint8_t pulseTimerOn = false;

    // Defining functions
    void initSendTimer();
    void initRecTimer();
    void initReceive();


    protected:

    private:
};

#endif
