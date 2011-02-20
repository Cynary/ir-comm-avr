#ifndef PHILLIPS_RECV_H
#define PHILLIPS_RECV_H

#define IRstart 0
#define IRstop 1
#define error 200

#define RPRES 64
#define RCPRES RPRES64
#define ROFF 0
#define RPRES1 1
#define RPRES8 2
#define RPRES64 3
#define RPRES256 4
#define RPRES1024 5
#define RusTIME_TOP(time) (((F_CPU/1000000L)*time)/RPRES)
#define ctime() (((TCNT0+ov*255)*RPRES)/(F_CPU/1000000))

extern volatile unsigned char recvflag;
extern volatile unsigned char timer0_use;
extern volatile int recvmsg;

void phillips_startRECV();
void phillips_stopRECV();

#endif
