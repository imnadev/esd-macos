
#include <avr/io.h>
#define F_CPU 16000000UL
#define BAUD 9600
//#define MYUBRR F_CPU/16/BAUD-1					//bit rate register with U2X=0
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "_main.h"
#include "_uart.h"

// Variables
unsigned char command = 0;						// 시리얼 통신 내용의 그에 따르는 내용을 할수 있도록 하는 변수
unsigned char InputSerialData = 0;				// UDR버퍼의 내용을 잠시 보관하는 변수
unsigned char Uart1_State = 0;  				// Key 값의 마지막을 기록 합니다.
unsigned int ubrr;

char Enter[]={"\r\n"};          				// Enter
char Tap[]={"\t"};              				// Tab

void Uart1_init(void)							// UART initialization
{
	// setting URT1 8N1 mode
	UCSR1A = 0x00;								// U2X=0 lowest baud rate for NYUBRR
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);			// character size 8
	UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1); // 0x98
	// RXCIE1(receive complete interrupt enable), RXEN1(receive enable), TXEN1(transmit enable)
	ubrr =  F_CPU/16/BAUD-1;
	UBRR1H = (ubrr>>8);				// MSB of MYUBRR
	UBRR1L = ubrr;					// LSB of MYUBRR
}


void putch_USART1(char data)					// 1byte 데이터 uart1으로 전송
{
	while( !(UCSR1A & (1<<UDRE1))); 			// 전송 인터럽트가 걸릴 때까지 while문 반복
	UDR1 = data; 								// 데이터 값을 UDR1에 넣는다. = 전송
}

void puts_USART1(char *str)						// 문자열 출력 루틴
{
	while( *str != 0)		 					// 문자의 마지막에는 ‘\0’이 들어가 있으므로
	{
		putch_USART1(*str);						// ‘\0’이 나올 때까지 출력한다.
		str++;
	}
}

// 아스키 값을 출력 하기 위한 테이블 작성
unsigned char TABLE[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void USART1_putchdecu(unsigned int dt)			// unsigned int 형 0~62536까지 표현 가능함 .
{
	unsigned int tmp;							// 만약 3245 값이 입력 되어 진다면 -> int형이기 때문에 03245가 됩니다.
	tmp = dt;
	putch_USART1(TABLE[tmp/10000]);				// 03245/10000=0.3256 -> 은 int형이기 때문에 0만 남게되며 TABLE의 첫번째 자리의 0이 출력 됩니다.
	tmp %= 10000;								// 03245/10000의 나머지 3256을 tmp에 저장합니다.
	putch_USART1(TABLE[tmp/1000]);				// 3245/1000=3.256 -> 3만 남게되며 TABLE의 두번째 자리의 3이 출력 됩니다.
	tmp %= 1000;								// 3245/1000의 나머지 245을 tmp에 저장합니다.
	putch_USART1(TABLE[tmp/100]);				// 245/100=2.45 -> 2만 남게되며 TABLE의 두번째 자리의 2가 출력 됩니다.
	tmp %= 100;									// 245/100의 나머지 45를 tmp에 저장합니다.
	putch_USART1(TABLE[tmp/10]);				// 45/10=4.5 -> 은 4만 남게되며 TABLE의 두번째 자리의 4가 출력 됩니다.
	putch_USART1(TABLE[tmp%10]);				// 나머지 5를 출력 합니다.
}

void USART1_putchuchar(unsigned char dt)		// unsigned char 형 0 ~ 255까지 표현 가능함 .
{
	unsigned char tmp;
	tmp = dt;
	putch_USART1(TABLE[tmp/100]);
	tmp %= 100;
	putch_USART1(TABLE[tmp/10]);
	putch_USART1(TABLE[tmp%10]);
}

unsigned int H2C(unsigned char ch) 				// character to HEX
{
	unsigned char high, low;
	unsigned int s;

	high = ch >> 4;
	low = ch & 0x0f;
	s = TABLE[high];
	s <<= 4;									// 8?
	s |= TABLE[low];

	return s;
}


void USART1_putchdecs(signed int dt)			// unsigned int  -32768 ~ 32767
{
	signed int tmp;
	tmp = dt;
	if(tmp >= 0) {
		putch_USART1('+');
		putch_USART1(TABLE[tmp/10000]);
		tmp %= 10000;
		putch_USART1(TABLE[tmp/1000]);
		tmp %= 1000;
		putch_USART1(TABLE[tmp/100]);
		tmp %= 100;
		putch_USART1(TABLE[tmp/10]);
		putch_USART1(TABLE[tmp%10]);
	}
	else {
		tmp = -tmp;
		putch_USART1('-');
		putch_USART1(TABLE[tmp/10000]);
		tmp %= 10000;
		putch_USART1(TABLE[tmp/1000]);
		tmp %= 1000;
		putch_USART1(TABLE[tmp/100]);
		tmp %= 100;
		putch_USART1(TABLE[tmp/10]);
		putch_USART1(TABLE[tmp%10]);
	}
}

void USART1_putchlongs(long dt)					// long type -2147483648 ~ 2147483647
{
	long tmp;
	tmp = dt;
	if(tmp >= 0) {
		putch_USART1('+');
		putch_USART1(TABLE[tmp/10000000]);
		tmp %= 10000000;
		putch_USART1(TABLE[tmp/1000000]);
		tmp %= 1000000;
		putch_USART1(TABLE[tmp/100000]);
		tmp %= 100000;
		putch_USART1(TABLE[tmp/10000]);
		tmp %= 10000;
		putch_USART1(TABLE[tmp/1000]);
		tmp %= 1000;
		putch_USART1(TABLE[tmp/100]);
		tmp %= 100;
		putch_USART1(TABLE[tmp/10]);
		putch_USART1(TABLE[tmp%10]);
	}
	else {
		tmp = -tmp;
		putch_USART1('-');
		putch_USART1(TABLE[tmp/10000000]);
		tmp %= 10000000;
		putch_USART1(TABLE[tmp/1000000]);
		tmp %= 1000000;
		putch_USART1(TABLE[tmp/100000]);
		tmp %= 100000;
		putch_USART1(TABLE[tmp/10000]);
		tmp %= 10000;
		putch_USART1(TABLE[tmp/1000]);
		tmp %= 1000;
		putch_USART1(TABLE[tmp/100]);
		tmp %= 100;
		putch_USART1(TABLE[tmp/10]);
		putch_USART1(TABLE[tmp%10]);
	}
}


void USART1_puthex(unsigned char dt)			// 핵사 파일을 출력으로 나타 냅니다.
{
	unsigned int  tmp=0;
	tmp = H2C(dt);
	putch_USART1((unsigned char)(tmp>>8));
	putch_USART1((unsigned char)(tmp));
}
/*
SIGNAL(USART1_RX_vect)							// Serial ISR
{
	cli(); //disable all interrupts
	
	//InputSerialData = UDR1 ;								// receive data with UDR
	//putch_USART1(InputSerialData);
	UDR1 = UDR1;
	//if(InputSerialData == '1') 		{	command = '1'; }	// HELP command
	//else if(InputSerialData == '2')	{	command = '2';	}	// Port Test command
	//
	sei();													//re-enable interrupts
}
*/


void Serial_Main(void)										// serial communication main
{
	//putch_USART1('a');
	if(command=='1')										// Help command
	{	Uart1_State = '1';
		////////////////////////////////////////////////////////////////////
		// task for keyboard 1
		PORTB = 0x00;
		//putch_USART1('a');
		puts_USART1("You typed 1\r\n");
		Uart1_DataView = 0x00;
		
		//-----------------------------------------------------------------//
		command = 0; 										// initialize variable
		InputSerialData=0;									// initialize variable for UDR buffer

	}	/////////////////////////////////////////////////////////////////// end else

	else if(command=='2')									// Port Test	
	{
		Uart1_State = '2';
		////////////////////////////////////////////////////////////////////
		// task for keyboard 2
		PORTB = 0xFF;
		//putch_USART1('b');
		puts_USART1("You typed 2\r\n");
		Uart1_DataView = 0xFF;
		
		//-----------------------------------------------------------------//
		command = 0; 										// initialize variable
		InputSerialData=0;									// initialize variable for UDR buffer

	}	/////////////////////////////////////////////////////////////////// end else

}