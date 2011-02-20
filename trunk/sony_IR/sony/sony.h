#ifndef SONY_H
#define SONY_H

/* Sony infrared remote protocol
 * Header file
 * Details about the protocol:
 * total: How many bits per message
 * start_bit: HIGH time for the start bit
 * guardtime: LOW time between bits (although a bit is defined as a pair of LOW/HIGH times, since the LOW time is constant for both 1 and 0, it's as if it separates them)
 * interval: minimum time between two messages (40ms)
 * bin: HIGH times for 0 and 1.
 */

// Protocol details:
#define start_bit 2400
#define guardtime 600
#define total 12
#define interval 40000
static const int bin[2] = { 600, 1200 };

#endif
