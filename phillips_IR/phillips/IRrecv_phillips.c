#ifndef IR_RECV
#define IR_RECV

#include <avr/io.h>
#include <avr/interrupt.h>
#include "phillips.h"
#include "IRrecv_phillips.h"
#include "../serial.h"

volatile unsigned char recvflag;
volatile int recvmsg;
int ov;
unsigned char sbit;
unsigned char current_bit;
volatile unsigned char edge;

static unsigned char TCCR0B_bits[] = {0, (1<<CS00), (1<<CS01), (1<<CS00) | (1<<CS01), (1<<CS02), (1<<CS00) | (1<<CS02)};

void phillips_startRECV() {
  if(timer0_use) {
    return;
  }
  EICRA = (1<<ISC00);
  EIMSK |= (1<<INT0);

  TCCR0B = TCCR0B_bits[RCPRES];
  TIMSK0 |= (1<<TOIE0);
  timer0_use = 1;
  sbit = 0;
}

void phillips_stopRECV() {
  TIMSK0 &= ~(1<<TOIE0);
  EIMSK &= ~(1<<INT0);
  timer0_use = 0;
  sbit = 0;
}

ISR(TIMER0_OVF_vect) {
  ++ov;
}

ISR(TIMER0_COMPA_vect) {
  if(PIND&(1<<PD2)) {
    edge = 1;
  }
  else {
    edge = 0;
  }
  if(sbit) {
    if(edge == IRstart) {
      OCR0A = RusTIME_TOP(bittime);
      sbit = 0;
      TCNT0 = 0;
    }
    else {
      EIMSK |= (1<<INT0);
      TCCR0A = 0;
      TIMSK0 &= ~(1<<OCIE0A);
    }
  }
  else if(current_bit == total) { // Received last bit already
    recvflag = 1;
    TCCR0A = 0;
    TIMSK0 &= ~(1<<OCIE0A);
    EIMSK |= (1<<INT0);
  }
  else { // Received a bit
    recvmsg |= (((edge==IRstart)?1:0)<<(current_bit++));
    TCNT0 = 0;
    OCR0A = RusTIME_TOP(bittime);
  }
}

ISR(INT0_vect) {
  int j;
  if(PIND&(1<<PD2)) {
    edge = 1;
  }
  else {
    edge = 0;
  }
  if(edge == IRstart) { // Start receiving a HIGH signal in IR, and count this time.
    TCNT0 = 0;
    ov = 0;
  }
  else { // Stopped receiving a HIGH signal -- we count the time from the start to the end, and see if it's the first start bit
    j = ctime();
    if(j > bittime/2-error && j < bittime/2+error) {
      sbit = 1;
      current_bit = 0;
      EIMSK &= ~(1<<INT0);
      TCCR0A |= (1<<WGM01);
      OCR0A = RusTIME_TOP(bittime*3/4);
      TCNT0 = 0;
      TIMSK0 |= (1<<OCIE0A);
      recvmsg = 0;
      recvflag = 0;
    }
  }
}

#endif
