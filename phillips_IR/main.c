#include <avr/io.h>
#include <avr/interrupt.h>
#include "phillips/IRsend_phillips.h"
#include "phillips/IRrecv_phillips.h"
#include "flags.h"
#include "serial.h"

int main(void) {
  USART_init();
  phillips_startRECV();
  sei();
  for(;;) {
    while(!recvflag) {
      phillips_send(149);
    }
    recvflag = 0;
    USART_sendDEC(recvmsg);
    USART_send('\n');
  }
}
