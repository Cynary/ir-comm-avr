#ifndef PHILLIPS_H
#define PHILLIPS_H

/* Phillips RC-5
 * This is a remote IR communication protocol.
 * It consists of 14 bits: 2 start bits, and 12 message bits.
 * With a period of 1778 us each bit, a low/high transition signals a 1, and a high/low transition signals a 0.
 */

#define bittime 1778
#define interval bittime*50
#define total 12

#endif
