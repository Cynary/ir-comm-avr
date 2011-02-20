#ifndef IR_SEND
#define IR_SEND

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../led.h"
#include "IRsend_sony.h"
#include "sony.h"

/* Functions to send a message through Infrared communication
 * Uses timer1 and associated interrupts to switch between bits.
 * Timer1 mode CTC.
 * Protocol: Sony infrared remote protocol
 * It sends 12 bits (7 bits of data, 5 bits of address).
 * Logical one: 600uS LOW, 1200uS HIGH
 * Logical zero: 600uS LOW, 600uS HIGH
 * Start bit: 2400uS HIGH
 */

volatile static int message;
static volatile unsigned char current_bit;
static volatile unsigned char pause;
volatile unsigned char ready_to_send;
unsigned char TIMERconfig;
unsigned char TCCR1B_bits[] = {(1<<WGM12), (1<<CS10) | (1<<WGM12), (1<<CS11) | (1<<WGM12), (1<<CS10) | (1<<CS11) | (1<<WGM12), (1<<CS12) | (1<<WGM12), (1<<CS12) | (1<<CS10) | (1<<WGM12)};

unsigned char configTIMER1() {
  if(timer1_use) {
    return 0;
  }
  ready_to_send = 1;
  TCCR1B = TCCR1B_bits[SCPRES];
  timer1_use = 1;
  TIMERconfig = 1;
  return 1;
}

void sony_send(int m) {
  message = m;
  if(!TIMERconfig) {
    if(!configTIMER1()) {
      return;
    }
  }
  if(!ready_to_send) {
    return;
  }
  ready_to_send = 0;
  current_bit = 0;
  pause = 1;
  OCR1A = usTIME_TOP(start_bit);
  onLED();
  TCNT1 = 0;
  TIFR1 |= (1<<OCF1A);
  TIMSK1 |= (1<<OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  if(current_bit == total) { // Last bit sent, start minimum interval -- 40 ms
    current_bit++;
    offLED();
    TCNT1 = 0;
    OCR1A = usTIME_TOP(interval);
  }

  else if(current_bit == total+1) { // Interval has passed, we can send again, and TIMER1 is made available
    TIMSK1 &= ~(1<<OCIE1A);
    TCCR1B = TCCR1B_bits[SOFF];
    TIMERconfig = 0;
    timer1_use = 0;
    ready_to_send = 1;
  }

  else if(pause) { // LOW time
    offLED();
    pause = 0;
    TCNT1 = 0;
    OCR1A = usTIME_TOP(guardtime);
  }

  else { // HIGH time
    onLED();
    pause = 1;
    TCNT1 = 0;
    OCR1A = usTIME_TOP(bin[((message>>(current_bit++))&1)]);
  }
}

#endif
