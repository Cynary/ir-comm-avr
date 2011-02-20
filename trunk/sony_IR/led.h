#ifndef LED_H
#define LED_H

/* LED Transmitter
 * Header File
 * Pin: OC2A/PB3
 * Frequency: LFREQ
 * Uses timer2 in CTC mode.
 * This IR LED will transmit the signal.
 */

// Frequency Settings:
#define LFREQ 38000L
#define LPRES 1
#define LCPRES LPRES1
#define LOFF 0
#define LPRES1 1
#define LPRES8 2
#define LPRES32 3
#define LPRES64 4
#define LPRES128 5
#define LPRES256 6
#define LPRES1024 7
#define FREQ_TOP (F_CPU/(2L*LPRES*LFREQ))
// CTC Mode, toggle LED pin on compare match:
#define TCCR2A_bits ((1<<COM2A0) | (1<<WGM21))

extern volatile unsigned char timer2_use;

unsigned char configLED();
void onLED();
void offLED();

#endif
