#include <avr/io.h>
#include "led.h"

/* LED Transmitter
 * Pin: OC2A/PB3
 * Frequency: LFREQ
 * Uses timer2 in CTC mode.
 * This IR LED will transmit the signal.
 */

unsigned char LED_config;

unsigned char TCCR2B_bits[] = {0, (1<<CS20), (1<<CS21), (1<<CS20) | (2<<CS21), (1<<CS22), (1<<CS20) | (1<<CS22), (1<<CS21) | (1<<CS22), (1<<CS20) | (1<<CS21) | (1<<CS22)};

// Configure timer2 to use an LED.
unsigned char configLED() {
  if(timer2_use) {
    return 0;
  }
  DDRB |= (1<<PB3); // LED Pin configure as output
  TCCR2A = TCCR2A_bits;
  OCR2A = FREQ_TOP;
  LED_config = 1;
  timer2_use = 1;
  return 1;
}

// Start emitting light at LFREQ Hz
void onLED() {
  if(!LED_config) {
    if(!configLED()) {
      return;
    }
  }
  TCCR2B = TCCR2B_bits[LCPRES];
}

// Stop emitting light
void offLED() {
  if(!LED_config) {
    if(!configLED()) {
      return;
    }
  }
  TCCR2B = TCCR2B_bits[LOFF];
  PORTB &= ~(1<<PB3);
  LED_config = 0;
  timer2_use = 0;
}
