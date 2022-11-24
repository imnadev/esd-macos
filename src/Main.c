/*
20211122 by Hong Jeong, IUT
https://ww1.microchip.com/downloads/en/DeviceDoc/doc2467.pdf
*/

/* Interrupts */
#define F_CPU 16000000UL
#include <avr/io.h>				//<avr/portpins.h> <avr/iom128.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "_main.h"
#include "_buzzer.h"
#include "_adc.h"
#include "_eeprom.h"
#include "_init.h"
//#include "_interrupt.h"
#include "_port.h"
#include "_timer2.h"
#include "_uart.h"
#include "_glcd.h"

/* 1. External Interrupt 
   1. External INT0~3: PD0~PD3, INT4~7: PE4~PE7
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

char a = 70;
char b = 2;
char buffer[20];
unsigned int result;


int main_interrupt (void) {
	init_devices();
	lcd_clear();
	lcd_string(0, 0, "U1910049 & N-1");
	lcd_string(1, 0, "Rustam & Sokhib");

	//apps
	DDRB = 0xFF;
	PORTB = 0xAA;
	
	//interrupt
	EICRA = 1<<ISC01;		//INT0 falling
	EICRB = 0x00;
	EIMSK = 1<<INT0;	//INT0 enabled
	sei();				//Interrupt enabled
	//apps
	
	char buffer[20];
	char hours = 0, minutes = 0, seconds = 0;
	while (1) {
		sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
		_delay_ms(1000);
		seconds += 1;
		minutes += seconds / 60;
		seconds %= 60;
		hours += minutes / 60;
		minutes %= 60;
		
		// lcd_string(3, 0, buffer);
	}	
	return 0;
}

SIGNAL (INT0_vect){
	PORTB = ~PORTB;
	
	result = a * b;
	
	// Printing into LCD
	lcd_string(5, 0, "");

	// Formatting the string
	sprintf(buffer, "%d = %u", a, result);
 
	a = a * 2;

	// Printing into the LCD
	lcd_string(5, 0, buffer);
}


/* 2. Timer Interrupt 
   1. TCNT0
   2. TCCR0: FOC0 WGM00 COM01 CON00 WGM01 CS02 CS01 CS00
   3. TIFR:  OCF2 TOV2  ICF1  OCF1B TOV1  OCF0 TOV0
      TIMSK: OCIE2 TOIE2 TICIE1 OCIE1A OCIE1B TOIE1 OCIE0 TOIE0
   4. SREG: SEI 
*/
/*
int main_interrupt (void)
{
	//apps
	DDRB = 0xFF;
	PORTB = 0xAA;
	//timer interrupt
	TCNT0 = 0x00;					// 0~255
	TCCR0 = (1<<CS02) | (1<< CS01) | (1<< CS00); // clock/1024
	TIMSK = 1<<TOIE0;
	while (1){
		sei();
		_delay_ms(500);
		cli();
		_delay_ms(500);
	}
	
	//apps
	while (1) {}	
	return 0;
}
SIGNAL (TIMER0_OVF_vect){
	PORTB = ~PORTB;
}
*/

/* 3. Timer Interrupt: CTC mode
   1. TCNT0, OCR0
   2. TCCR0: FOC0 WGM00 COM01 CON00 WGM01 CS02 CS01 CS00
   3. TIFR:  OCF2 TOV2  ICF1  OCF1B TOV1  OCF0 TOV0
      TIMSK: OCIE2 TOIE2 TICIE1 OCIE1A OCIE1B TOIE1 OCIE0 TOIE0
   4. SREG: SEI 
*/
/*
int main_interrupt (void)
{
	//apps
	DDRB = 0xFF;
	PORTB = 0xAA;
	//timer interrupt
	TCNT0 = 0x00;
	OCR0 = 0xFF;					// 0~255
	TCCR0 = (1<<WGM01) | (1<<CS02) | (1<< CS01) | (1<< CS00); // CTC and Clock/1024
	TIMSK = 1<<OCIE0;
	while (1){
		sei();
		_delay_ms(500);
		cli();
		_delay_ms(500);
	}
	
	//apps
	while (1) {}	
	return 0;
}
SIGNAL (TIMER0_COMP_vect){
	PORTB = ~PORTB;
}
*/



int main (void)
{
	main_interrupt();
}