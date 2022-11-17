#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "_main.h"
#include "_interrupt.h"
#include "_uart.h"

/* 1. External INT0~3: PD0~PD3, INT4~7: PE4~PE7
   2. EICRA: ISC31 ISC30 ISC21 ISC20 ISC11 ISC10 ISC01 ISC00
	  EICRB: ISC71 ISC70 ISC61 ISC60 ISC51 ISC50 ISC41 ISC40
   ISCn1 ISCn0 Description
	0 0 The low level of INTn generates an interrupt request.
	0 1 Reserved
	1 0 The falling edge of INTn generates asynchronously an interrupt request.
	1 1 The rising edge of INTn generates asynchronously an interrupt request.
   3. EIMSK: INT7 INT6 INT5 INT4 INT3 INT2 INT1 INT0
   4. SREG: SEI 
*/
void Interrupt_init(void) 	// Port initialization
{
	EICRA = 0x0A; 		// INT0~1, Falling edge
	EICRB = 0x00; 		// INT4~7, Unused
	EIMSK = (1<<INT1) | (1<<INT0) ; // EIMSK = 0x03;
}
/*
SIGNAL(INT0_vect)	
{
	putch_USART1(0x41);	// putch_USART1('a');
	INT0_DataView++;
}
*/
/*
SIGNAL(INT1_vect)	
{
	putch_USART1(0x42);	// putch_USART1('b');
	INT1_DataView++;
}
*/