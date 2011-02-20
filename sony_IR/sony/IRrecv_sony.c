#ifndef IR_RECV
#define IR_RECV

#include <avr/io.h>
#include <avr/interrupt.h>
#include "sony.h"
#include "IRrecv_sony.h"

/* Functions to receive data through IR
 * Uses external interrupts to know when data arrives.
 * Through timer0, it knows how long the transmission has been on either HIGH or LOW -- uses this, associated with a permitted error, to know if it receives a start bit, a 0, a 1, or if there's been an error.
 * To measure time, it uses the value in timer0, and an overflow flag which is incremented through an interruption of timer0. This timer will be used in Normal mode.
 * When it receives a full transmission, it sets the recvflag variable, and stores the message in the global variable recvmsg.
 * Connect IR receiver in pin INT0/PD2, and set the IRstop/IRstart constants according to its type.
 */

volatile unsigned char recvflag;
volatile int recvmsg;
int ov;
unsigned char sbit;
unsigned char current_bit;
volatile unsigned char edge;

unsigned char TCCR0B_bits[] = {0, (1<<CS00), (1<<CS01), (1<<CS00) | (1<<CS01), (1<<CS02), (1<<CS00) | (1<<CS02)};

void sony_startRECV() {
  EICRA = (1<<ISC00);
  EIMSK |= (1<<INT0);

  TCCR0B = TCCR0B_bits[RCPRES];
  TIMSK0 |= (1<<TOIE0);
  timer0_use = 1;
  sbit = 0;
}

void sony_stopRECV() {
  TIMSK0 &= ~(1<<TOIE0);
  EIMSK &= ~(1<<INT0);
  timer0_use = 0;
  sbit = 0;
}

ISR(TIMER0_OVF_vect) {
  ++ov;
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
  else { // Stopped receiving a HIGH signal -- we count the time from the start to the end, and interpret it as 0, 1, start bit or an error.
    j = ctime();
    if(current_bit == total) {
      recvflag = 1;
      current_bit = 0;
      sbit = 0;
    }
    else if(sbit) {
      if(j > bin[0]-error && j < bin[0]+error) { // received bit 0
        recvmsg &= ~(1<<(current_bit++));
      }
      else if(j > bin[1]-error && j < bin[1]+error) { // received bit 0
        recvmsg |= (1<<(current_bit++));
      }
      else { // error
        recvmsg = 0;
        sbit = 0;
        current_bit = 0;
      }
    }
    else if(j > start_bit-error && j < start_bit+error) {
      sbit = 1;
      current_bit = 0;
    }
  }
}

#endif
