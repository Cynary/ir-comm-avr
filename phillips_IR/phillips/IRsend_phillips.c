#ifndef IR_SEND
#define IR_SEND

#include "IRsend_phillips.h"
#include "phillips.h"
#include "../led.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char ready_to_send;
volatile static int message;
volatile static unsigned char current_bit;
volatile static unsigned char pause;
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

void phillips_send(int msg) {
  message = (msg<<2) | 0b11;
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
  OCR1A = usTIME_TOP(bittime/2);
  onLED();
  TCNT1 = 0;
  TIFR1 |= (1<<OCF1A);
  TIMSK1 |= (1<<OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  if(current_bit == total+2) { // Last bit sent, start minimum interval
    current_bit++;
    offLED();
    TCNT1 = 0;
    OCR1A = usTIME_TOP(interval);
  }

  else if(current_bit == total+3) { // Interval has passed, we can send again, and TIMER1 is made available
    TIMSK1 &= ~(1<<OCIE1A);
    TCCR1B = TCCR1B_bits[SOFF];
    TIMERconfig = 0;
    timer1_use = 0;
    ready_to_send = 1;
  }

  else if(pause) { // TOGGLE time
    if((message>>current_bit)&1) {
      offLED();
    }
    else {
      onLED();
    }
    pause = 0;
    TCNT1 = 0;
    OCR1A = usTIME_TOP(bittime/2);
  }

  else { // BIT time
    if((message>>current_bit)&1) {
      onLED();
    }
    else {
      offLED();
    }
    pause = 1;
    TCNT1 = 0;
    current_bit++;
    OCR1A = usTIME_TOP(bittime/2);
  }
}

#endif
