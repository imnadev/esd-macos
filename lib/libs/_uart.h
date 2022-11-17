
void Uart1_init(void);
void putch_USART1(char data);
void puts_USART1(char *str);
void USART1_putchdecu(unsigned int dt);
void USART1_putchuchar(unsigned char dt);
unsigned int H2C(unsigned char ch);
void USART1_putchdecs(signed int dt);
void USART1_putchlongs(long dt);
void USART1_puthex(unsigned char dt);
SIGNAL(USART0_RX_vect);
void Serial_Main(void);

extern unsigned char command;			// 시리얼 통신 내용의 그에 따르는 내용을 할수 있도록 하는 변수
extern unsigned char InputSirialData;  // UDR버퍼의 내용을 잠시 보관하는 변수
extern unsigned char Uart1_State;  	// Key 값의 마지막을 기록 합니다.