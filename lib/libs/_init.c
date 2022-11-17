
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "_main.h"
#include "_buzzer.h"
#include "_adc.h"
#include "_eeprom.h"
#include "_init.h"
#include "_interrupt.h"
#include "_port.h"
#include "_timer2.h"
#include "_uart.h"
#include "_glcd.h"


void init_devices(void)	// 초기화 함수를 여기에 넣습니다.
{
	cli(); //disable all interrupts

	Port_init();		// Port 초기화
	Interrupt_init();
	Timer2_init();  	// 타이머/카운터2의 초기화
	Uart1_init();
	Adc_init();
	lcd_init();          // initialize GLCD
		
	sei(); //re-enable interrupts
}