#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "_main.h"
#include "_port.h"


void Port_init(void) 						// 포트 초기화 구문입니다.
{
	//PORTA = 0x00; 	DDRA = 0xFF;			// PORTA 출력 LOW ,핀의 출력 설정
	//PORTB = 0xFF; 	DDRB = 0b11111111;		// PORTB 출력 LOW ,핀의 출력 설정
	//PORTC = 0x00; 	DDRC = 0xF0;			// PORTC 상위 4bit 입력, 하위 4bit 출력
	//PORTD = 0x80;	DDRD = 0b10000000;		// 핀의 입출력 설정
	//PORTE = 0x00; 	DDRE = 0xFF;			// PORTE 출력 LOW ,핀의 출력 설정
	//PORTF = 0x00; 	DDRF = 0x00;			// PORTE 출력 LOW ,핀의 출력 설정
	
	PORTA = 0x00; 	DDRA = 0xFF;			// PA: D
	PORTB = 0xFF; 	DDRB = 0xFF;			// PB: LED
	PORTD = 0x00;	DDRD = 0x00;			// PD: button
	PORTE = 0x00; 	DDRE = 0xFF;			// PE4: R/S, PE5: E, PE6: CS2, PE7: CS1
	PORTG = 0x00;	DDRG = 0xFF;			// PG4: Buzzer
}
