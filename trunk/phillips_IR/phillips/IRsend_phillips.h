#ifndef PHILLIPS_SEND_H
#define PHILLIPS_SEND_H

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
void phillips_send(int);

#endif
