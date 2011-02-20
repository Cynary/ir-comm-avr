#ifndef SONY_SEND_H
#define SONY_SEND_H

/* Functions to send a message through Infrared communication
 * Uses timer1 and associated interrupts to switch between bits
 * Protocol: Sony infrared remote protocol
 * It sends 12 bits (7 bits of data, 5 bits of address).
 * Logical one: 600uS LOW, 1200uS HIGH
 * Logical zero: 600uS LOW, 600uS HIGH
 * Header file
 * Start bit: 2400uS HIGH
 */

#define SPRES 256
#define SCPRES SPRES256
#define SOFF 0
#define SPRES1 1
#define SPRES8 2
#define SPRES64 3
#define SPRES256 4
#define SPRES1024 5
#define usTIME_TOP(time) (((F_CPU/1000000L)*time)/SPRES)


extern volatile unsigned char timer1_use;
extern volatile unsigned char ready_to_send;
void sony_send(int);

#endif
