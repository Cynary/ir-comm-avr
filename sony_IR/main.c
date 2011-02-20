#include <avr/io.h>
#include <avr/interrupt.h>
#include "sony/IRsend_sony.h"
#include "sony/IRrecv_sony.h"
#include "flags.h"
#include "serial.h"

int main(void) {
  USART_init();
  sony_startRECV();
  sei();
  for(;;) {
    while(!recvflag) {
      sony_send(149);
    }
    recvflag = 0;
    USART_sendDEC(recvmsg);
    USART_send('\n');
  }
}
