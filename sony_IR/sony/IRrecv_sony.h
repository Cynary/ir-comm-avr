#ifndef SONY_RECV_H
#define SONY_RECV_H

/* Functions to receive data through IR
 * Header File
 * Uses external interrupts to know when data arrives.
 * Through timer0, it knows how long the transmission has been on either HIGH or LOW -- uses this, associated with a permitted error, to know if it receives a start bit, a 0, a 1, or if there's been an error.
 * To measure time, it uses the value in timer0, and an overflow flag which is incremented through an interruption of timer0. This timer will be used in Normal mode.
 * When it receives a full transmission, it sets the recvflag variable, and stores the message in the global variable recvmsg.
 * Connect IR receiver in pin INT0/PD2, and set the IRstop/IRstart constants according to its type.
 */

#define IRstart 0
#define IRstop 1
#define error 200

#define RPRES 8
#define RCPRES RPRES8
#define ROFF 0
#define RPRES1 1
#define RPRES8 2
#define RPRES64 3
#define RPRES256 4
#define RPRES1024 5
#define ctime() (((TCNT0+ov*255)*RPRES)/(F_CPU/1000000))

extern volatile unsigned char recvflag;
extern volatile unsigned char timer0_use;
extern volatile int recvmsg;

void sony_startRECV();
void sony_stopRECV();

#endif
