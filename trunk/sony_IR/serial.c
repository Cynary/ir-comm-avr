#include <avr/io.h>
#include "serial.h"

void USART_init(void) {
	UBRR0H = (unsigned char) (BAUD_PRESCALLER >> 8);
	UBRR0L = (unsigned char) (BAUD_PRESCALLER);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (3 << UCSZ00);
}
unsigned char USART_receive(void) {
	while (!(UCSR0A & (1 << RXC0)))
		;
	return UDR0;
}

void USART_send(unsigned char data) {
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = data;
}
void USART_putstring(char* StringPtr) {
	while (*StringPtr != 0x00) {
		USART_send(*StringPtr);
		StringPtr++;
	}
}

void USART_sendDEC(int dec) {
  if(dec == 0) {
    USART_send('0');
    return;
  }
	char n[20];
	int i = 0;
	if(dec < 0) {
	  USART_send('-');
	  dec = 0-dec;
	}
	while(dec) {
		n[i++] = (dec%10)+'0';
		dec /= 10;
	}
	for(--i; i >= 0; --i) {
		USART_send(n[i]);
	}
}
