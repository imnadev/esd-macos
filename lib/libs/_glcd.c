#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "_main.h"
#include "_glcd.h"

typedef  unsigned char byte;
typedef  unsigned int  word;

#define  D_BEFORE 0
#define  D_AFTER  10
#define  D_MIDDLE 0
#define  DISPON   0x3f
#define  DISPOFF  0x3e
word   d;

byte   xchar, ychar;				   /* x character(0-7), y character(0-19) */

byte font[95][5] = {				   /* 5x7 ASCII character font draw a byte downwards */
	{0x00, 0x00, 0x00, 0x00, 0x00},    /* 0x20 space */
	{0x00, 0x00, 0x4f, 0x00, 0x00},    /* 0x21 ! */
	{0x00, 0x07, 0x00, 0x07, 0x00},    /* 0x22 " */
	{0x14, 0x7f, 0x14, 0x7f, 0x14},    /* 0x23 # */
	{0x24, 0x2a, 0x7f, 0x2a, 0x12},    /* 0x24 $ */
	{0x23, 0x13, 0x08, 0x64, 0x62},    /* 0x25 % */
	{0x36, 0x49, 0x55, 0x22, 0x50},    /* 0x26 & */
	{0x00, 0x05, 0x03, 0x00, 0x00},    /* 0x27 ' */
	{0x00, 0x1c, 0x22, 0x41, 0x00},    /* 0x28 ( */
	{0x00, 0x41, 0x22, 0x1c, 0x00},    /* 0x29 ) */
	{0x14, 0x08, 0x3e, 0x08, 0x14},    /* 0x2a * */
	{0x08, 0x08, 0x3e, 0x08, 0x08},    /* 0x2b + */
	{0x00, 0x50, 0x30, 0x00, 0x00},    /* 0x2c , */
	{0x08, 0x08, 0x08, 0x08, 0x08},    /* 0x2d - */
	{0x00, 0x60, 0x60, 0x00, 0x00},    /* 0x2e . */
	{0x20, 0x10, 0x08, 0x04, 0x02},    /* 0x2f / */
	{0x3e, 0x51, 0x49, 0x45, 0x3e},    /* 0x30 0 */
	{0x00, 0x42, 0x7f, 0x40, 0x00},    /* 0x31 1 */
	{0x42, 0x61, 0x51, 0x49, 0x46},    /* 0x32 2 */
	{0x21, 0x41, 0x45, 0x4b, 0x31},    /* 0x33 3 */
	{0x18, 0x14, 0x12, 0x7f, 0x10},    /* 0x34 4 */
	{0x27, 0x45, 0x45, 0x45, 0x39},    /* 0x35 5 */
	{0x3c, 0x4a, 0x49, 0x49, 0x30},    /* 0x36 6 */
	{0x01, 0x71, 0x09, 0x05, 0x03},    /* 0x37 7 */
	{0x36, 0x49, 0x49, 0x49, 0x36},    /* 0x38 8 */
	{0x06, 0x49, 0x49, 0x29, 0x1e},    /* 0x39 9 */
	{0x00, 0x36, 0x36, 0x00, 0x00},    /* 0x3a : */
	{0x00, 0x56, 0x36, 0x00, 0x00},    /* 0x3b ; */
	{0x08, 0x14, 0x22, 0x41, 0x00},    /* 0x3c < */
	{0x14, 0x14, 0x14, 0x14, 0x14},    /* 0x3d = */
	{0x00, 0x41, 0x22, 0x14, 0x08},    /* 0x3e > */
	{0x02, 0x01, 0x51, 0x09, 0x06},    /* 0x3f ? */
	{0x32, 0x49, 0x79, 0x41, 0x3e},    /* 0x40 @ */
	{0x7e, 0x11, 0x11, 0x11, 0x7e},    /* 0x41 A */
	{0x7f, 0x49, 0x49, 0x49, 0x36},    /* 0x42 B */
	{0x3e, 0x41, 0x41, 0x41, 0x22},    /* 0x43 C */
	{0x7f, 0x41, 0x41, 0x22, 0x1c},    /* 0x44 D */
	{0x7f, 0x49, 0x49, 0x49, 0x41},    /* 0x45 E */
	{0x7f, 0x09, 0x09, 0x09, 0x01},    /* 0x46 F */
	{0x3e, 0x41, 0x49, 0x49, 0x7a},    /* 0x47 G */
	{0x7f, 0x08, 0x08, 0x08, 0x7f},    /* 0x48 H */
	{0x00, 0x41, 0x7f, 0x41, 0x00},    /* 0x49 I */
	{0x20, 0x40, 0x41, 0x3f, 0x01},    /* 0x4a J */
	{0x7f, 0x08, 0x14, 0x22, 0x41},    /* 0x4b K */
	{0x7f, 0x40, 0x40, 0x40, 0x40},    /* 0x4c L */
	{0x7f, 0x02, 0x0c, 0x02, 0x7f},    /* 0x4d M */
	{0x7f, 0x04, 0x08, 0x10, 0x7f},    /* 0x4e N */
	{0x3e, 0x41, 0x41, 0x41, 0x3e},    /* 0x4f O */
	{0x7f, 0x09, 0x09, 0x09, 0x06},    /* 0x50 P */
	{0x3e, 0x41, 0x51, 0x21, 0x5e},    /* 0x51 Q */
	{0x7f, 0x09, 0x19, 0x29, 0x46},    /* 0x52 R */
	{0x26, 0x49, 0x49, 0x49, 0x32},    /* 0x53 S */
	{0x01, 0x01, 0x7f, 0x01, 0x01},    /* 0x54 T */
	{0x3f, 0x40, 0x40, 0x40, 0x3f},    /* 0x55 U */
	{0x1f, 0x20, 0x40, 0x20, 0x1f},    /* 0x56 V */
	{0x3f, 0x40, 0x38, 0x40, 0x3f},    /* 0x57 W */
	{0x63, 0x14, 0x08, 0x14, 0x63},    /* 0x58 X */
	{0x07, 0x08, 0x70, 0x08, 0x07},    /* 0x59 Y */
	{0x61, 0x51, 0x49, 0x45, 0x43},    /* 0x5a Z */
	{0x00, 0x7f, 0x41, 0x41, 0x00},    /* 0x5b [ */
	{0x02, 0x04, 0x08, 0x10, 0x20},    /* 0x5c \ */
	{0x00, 0x41, 0x41, 0x7f, 0x00},    /* 0x5d ] */
	{0x04, 0x02, 0x01, 0x02, 0x04},    /* 0x5e ^ */
	{0x40, 0x40, 0x40, 0x40, 0x40},    /* 0x5f _ */
	{0x00, 0x01, 0x02, 0x04, 0x00},    /* 0x60 ` */
	{0x20, 0x54, 0x54, 0x54, 0x78},    /* 0x61 a */
	{0x7f, 0x48, 0x44, 0x44, 0x38},    /* 0x62 b */
	{0x38, 0x44, 0x44, 0x44, 0x20},    /* 0x63 c */
	{0x38, 0x44, 0x44, 0x48, 0x7f},    /* 0x64 d */
	{0x38, 0x54, 0x54, 0x54, 0x18},    /* 0x65 e */
	{0x08, 0x7e, 0x09, 0x01, 0x02},    /* 0x66 f */
	{0x0c, 0x52, 0x52, 0x52, 0x3e},    /* 0x67 g */
	{0x7f, 0x08, 0x04, 0x04, 0x78},    /* 0x68 h */
	{0x00, 0x04, 0x7d, 0x00, 0x00},    /* 0x69 i */
	{0x20, 0x40, 0x44, 0x3d, 0x00},    /* 0x6a j */
	{0x7f, 0x10, 0x28, 0x44, 0x00},    /* 0x6b k */
	{0x00, 0x41, 0x7f, 0x40, 0x00},    /* 0x6c l */
	{0x7c, 0x04, 0x18, 0x04, 0x7c},    /* 0x6d m */
	{0x7c, 0x08, 0x04, 0x04, 0x78},    /* 0x6e n */
	{0x38, 0x44, 0x44, 0x44, 0x38},    /* 0x6f o */
	{0x7c, 0x14, 0x14, 0x14, 0x08},    /* 0x70 p */
	{0x08, 0x14, 0x14, 0x18, 0x7c},    /* 0x71 q */
	{0x7c, 0x08, 0x04, 0x04, 0x08},    /* 0x72 r */
	{0x48, 0x54, 0x54, 0x54, 0x20},    /* 0x73 s */
	{0x04, 0x3f, 0x44, 0x40, 0x20},    /* 0x74 t */
	{0x3c, 0x40, 0x40, 0x20, 0x7c},    /* 0x75 u */
	{0x1c, 0x20, 0x40, 0x20, 0x1c},    /* 0x76 v */
	{0x3c, 0x40, 0x30, 0x40, 0x3c},    /* 0x77 w */
	{0x44, 0x28, 0x10, 0x28, 0x44},    /* 0x78 x */
	{0x0c, 0x50, 0x50, 0x50, 0x3c},    /* 0x79 y */
	{0x44, 0x64, 0x54, 0x4c, 0x44},    /* 0x7a z */
	{0x00, 0x08, 0x36, 0x41, 0x00},    /* 0x7b { */
	{0x00, 0x00, 0x77, 0x00, 0x00},    /* 0x7c | */
	{0x00, 0x41, 0x36, 0x08, 0x00},    /* 0x7d } */
	{0x08, 0x04, 0x08, 0x10, 0x08}};   /* 0x7e ~ */
		
	
	/* command output */

	// RS	pin PE4 [L:command, H:data]		PIN14
	// RW	pin GND [L write fixed]			GND
	// E	pin PE5 [L:off, H:on]			PIN1
	// CS1	pin PE7 [L:off, H:on]			PIN16
	// CS2	pin PE6 [L:off, H:on]			PIN17
	// DATA pin PORTA
	// SetBit(x,y);
	// ClrBit(x,y);
	
	void cmndl( byte cmd )			// left 128x64
	{	
		PORTA = cmd ;				// DATA pin PORTA	write vertical upwards at current character position
		ClrBit(PORTE,PORTE4);		// PORTE.4 = 0;		//	RS
		ClrBit(PORTE,PORTE6);		// PORTE.6 = 0;  
		SetBit(PORTE,PORTE7);		// PORTE.7 = 1;		// left pannel
		_delay_us(D_MIDDLE);
		SetBit(PORTE,PORTE5);		// PORTE.5 = 1;		//  E
		_delay_us(D_BEFORE);
		ClrBit(PORTE,PORTE5);		// PORTE.5 = 0;		// E
		_delay_us(D_AFTER);
	}
	
	void cmndr( byte cmd )			// right 128x64
	{
		PORTA = cmd ;				// DATA pin PORTA
		ClrBit(PORTE,PORTE4);		// RS
		SetBit(PORTE,PORTE6);		// PORTE.6 = 1;  right pannel
		ClrBit(PORTE,PORTE7);		// PORTE.7 = 0;
		_delay_us(D_MIDDLE);
		SetBit(PORTE,PORTE5);		// PORTE.5 = 1;		//  E
		_delay_us(D_BEFORE);
		ClrBit(PORTE,PORTE5);		// PORTE.5 = 0;		// E
		_delay_us(D_AFTER);
	}
	
	void cmnda( byte cmd )			// both 128x64
	{
		PORTA = cmd ;				// DATA pin PORTA			
		ClrBit(PORTE,PORTE4);		// RS
		SetBit(PORTE,PORTE6);	
		SetBit(PORTE,PORTE7);		// PORTE.6 = 1;  PORTE.7 = 1;	both pannels
		_delay_us(D_MIDDLE);	
		SetBit(PORTE,PORTE5);		// PORTE.5 = 1;		//  E
		_delay_us(D_BEFORE);
		ClrBit(PORTE,PORTE5);		// PORTE.5 = 0;		// E
		_delay_us(D_AFTER);
	}
	
	/* 1 character output  */	
	void datal( byte dat )          // left 128x64
	{
		PORTA = dat ;				// DATA pin PORTA
		SetBit(PORTE,PORTE4);		// PORTE.4 = 1;  // RS
		ClrBit(PORTE,PORTE6);
		SetBit(PORTE,PORTE7);		// PORTE.6 = 0;  PORTE.7 = 1;	
		_delay_us(D_MIDDLE);	
		SetBit(PORTE,PORTE5);		// PORTE.5 = 1;		//  E
		_delay_us(D_BEFORE);
		ClrBit(PORTE,PORTE5);		// PORTE.5 = 0;		// E
		_delay_us(D_AFTER);
	}
	
	void datar( byte dat )          // right 128x64
	{
		PORTA = dat ;				// DATA pin PORTA	
		SetBit(PORTE,PORTE4);		// PORTE.4 = 1;  // RS
		SetBit(PORTE,PORTE6);
		ClrBit(PORTE,PORTE7);		// PORTE.6 = 1;  PORTE.7 = 0;
		_delay_us(D_MIDDLE);
		SetBit(PORTE,PORTE5);		// PORTE.5 = 1;		//  E
		_delay_us(D_BEFORE);
		ClrBit(PORTE,PORTE5);		// PORTE.5 = 0;		// E
		_delay_us(D_AFTER);
	}
	
	void dataa( byte dat )          // both 128x64
	{
		PORTA = dat ;				// DATA pin PORTA
		SetBit(PORTE,PORTE4);		// PORTE.4 = 1;  // RS
		SetBit(PORTE,PORTE6);
		SetBit(PORTE,PORTE7);		// PORTE.6 = 1;  PORTE.7 = 1;
		_delay_us(D_MIDDLE);
		SetBit(PORTE,PORTE5);		// PORTE.5 = 1;		//  E
		_delay_us(D_BEFORE);
		ClrBit(PORTE,PORTE5);		// PORTE.5 = 0;		// E
		_delay_us(D_AFTER);	
	}
	
	/* GLCD Clear */
	void lcd_clear(void)			/* clear LCD screen */
	{
		byte i, j, x, y;
		x = 0xB8;					/* X start address Page 0*/
		y = 0x40;					/* Y start address Column 0*/
		for(i = 0; i <= 7; i++)
		{
			cmnda(x);    cmnda(y);
			for(j = 0; j <= 63; j++)
			dataa(0x00);			/* clear CS1 and CS2 */
			x++;
		}
	}

	
	/* GLCD Initialize */	
	void lcd_init(void)
	{
		cmnda( DISPON );	// Display ON
		cmnda( 0xc0 );		// z = 0 first line
		cmnda( 0xb8 );		// x = 0 first page
		cmnda( 0x40 );		// y = 0 first column
	}
	
	
	/* character position */
	void  lcd_xy( byte x, byte y )
	{
		xchar = x;							/* x = 0~7 */
		ychar = y;							/* y = 0~9 */
		cmnda(0xB8+xchar);					/* X address */
		if(ychar <= 9)						/* if y <= 9, CS1 Y address 8x10 characters for a pannel 4 offset */
			cmndl(0x40+ychar*6+4);			/* 5x7 -> 6x8 actually bottom for cursor right for empty vertical */
		else								/* if y >= 10, CS2 Y address */
			cmndr(0x40+(ychar-10)*6);
	}

	/* character output */
	void  lcd_char(byte character)
	{
		byte i;
		for(i = 0; i <= 4; i++)
		{
			if(ychar <= 9)							/* if y <= 9, CS1 */
				datal(font[character-0x20][i]);
			else									/* if y >= 10, CS2 */
				datar(font[character-0x20][i]);
		}
		if(ychar <=9)
			datal(0x00);							/* last byte 0x00 making 6x8 pixel per a character */
		else
			datar(0x00);							/* last byte 0x00 making 6x8 pixel per a character*/
	}


	/* character sequence output */
	void  lcd_string(byte x,byte y,char *string)
	{
		xchar = x;
		ychar = y;
		lcd_xy(x,y);
		while(*string != '\0')				/* null */
		{
			if(ychar == 10)                  /* change from CS1 to CS2 */
				cmndr(0x40);
			lcd_char(*string);               /* display a character */
			string++;                        /* next character */
			ychar++;                         /* next line */
		}
	}

	void GLCD_Axis_xy(unsigned char x, unsigned char y)	// draw a byte downwards
	{
		cmnda( 0xB8 + x);					// X address page
		if(y <= 63)
		{
			cmndl( 0x40 + y);				// CS1 Y address column
		}
		else
		{
			cmndr( 0x40 + y - 64);			// CS2 Y address
		}
	}

	unsigned char ScreenBuffer[8][128];		// screen buffer
	// draw a dot on GLCD
	void GLCD_Dot(unsigned char xx, unsigned char y)
	{
		unsigned char x, i;
		
		// check resolution (128.64)
		if((xx > 63) || (y > 127)) return;
		x = xx / 8;							// calculate x address
		i = xx % 8;							// pixel number in th x address downwards
		if(i == 0)      { i = 0x01; }		// top pixel
		else if(i == 1) { i = 0x02; }
		else if(i == 2) { i = 0x04; }
		else if(i == 3) { i = 0x08; }
		else if(i == 4) { i = 0x10; }
		else if(i == 5) { i = 0x20; }
		else if(i == 6) { i = 0x40; }
		else            { i = 0x80; }		// bottom pixel
		
		ScreenBuffer[x][y] |= i;			// OR old data with new data
		GLCD_Axis_xy(x, y);					// draw dot on GLCD screen
		if(y <= 63) { datal(ScreenBuffer[x][y]);  }
		else        { datar(ScreenBuffer[x][y]);  }
	}

	void ScreenBuffer_clear(void)
	{
		unsigned char i, j;
		
		for(i=0; i<8; i++)
		{
			for(j=0; j < 128; j++)
			{
				ScreenBuffer[i][j] = 0x00;
			}
		}

	}


	void GLCD_Line(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
	{
		int x, y;
		if(y1 != y2)						// if y1 != y2, y is variable
		{
			if(y1 < y2)						// x is function
			{
				for(y = y1; y <= y2; y++)
				{
					x = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
					GLCD_Dot(x,y);
				}
			}
			else
			{
				for(y = y1; y >= y2; y--)
				{
					x = x1 + (y - y1)*(x2 - x1)/(y2 - y1);
					GLCD_Dot(x,y);
				}
			}
		}
		else if(x1 != x2)					// if x1 != x2, x is variable
		{
			if(x1 < x2)						// y is function
			{
				for(x = x1; x <= x2; x++)
				{ y = y1 + (x - x1)*(y2 - y1)/(x2 - x1);
					GLCD_Dot(x,y);
				}
			}
			else
			{
				for(x = x1; x >= x2; x--)
				{
					y = y1 + (x - x1)*(y2 - y1)/(x2 - x1);
					GLCD_Dot(x,y);
				}
			}
		}
		else								// if x1 == x2 and y1 == y2,
		{
			GLCD_Dot(x1,y1);				// it is a dot
		}
	}


	// draw a rectangle
	void GLCD_Rectangle(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
	{
		GLCD_Line(x1,y1,x1,y2);				// horizontal line
		GLCD_Line(x2,y1,x2,y2);
		GLCD_Line(x1,y1,x2,y1);				// vertical line
		GLCD_Line(x1,y2,x2,y2);
	}

	// draw a circle
	void GLCD_Circle(unsigned char x1,unsigned char y1,unsigned char r)
	{
		int x, y;
		float s;
		for(y = y1 - r*3/4; y <= y1 + r*3/4; y++) // draw with y variable
		{
			s = sqrt(r*r - (y-y1)*(y-y1)) + 0.5;
			x = x1 + (unsigned char)s;
			GLCD_Dot(x,y);
			x = x1 - (unsigned char)s;
			GLCD_Dot(x,y);
		}
		for(x = x1 - r*3/4; x <= x1 + r*3/4; x++) // draw with x variable
		{
			s = sqrt(r*r - (x-x1)*(x-x1)) + 0.5;
			y = y1 + (unsigned char)s;
			GLCD_Dot(x,y);
			y = y1 - (unsigned char)s;
			GLCD_Dot(x,y);
		}
	}


	// display 1-digit decimal number
	unsigned char GLCD_1DigitDecimal(unsigned char number, unsigned char flag)
	{
		number %= 10;               // 10^0
		
		if ((number == 0) && (flag == 0))
		{
			lcd_char(' ');
			return 0;
		}
		
		lcd_char(number + '0');
		return 1;
	}

	// display 2-digit decimal number
	void GLCD_2DigitDecimal(unsigned char number)
	{
		unsigned int i;
		unsigned char flag;
		
		flag = 0;
		number = number % 100;
		i = number/10;
		flag = GLCD_1DigitDecimal(i, flag);			// 10^1
		i = number % 10;
		lcd_char(i + '0');							// 10^0
	}

	// display 3-digit decimal number
	void GLCD_3DigitDecimal(unsigned int number)
	{
		unsigned int i;
		unsigned char flag;
		flag = 0;
		number = number % 1000;
		i = number/100;
		flag = GLCD_1DigitDecimal(i, flag);			// 10^2
		number = number % 100;
		i = number/10;
		flag = GLCD_1DigitDecimal(i, flag);			// 10^1
		i = number % 10;
		lcd_char(i + '0');							// 10^0
	}

	// display 4-digit decimal number
	void GLCD_4DigitDecimal(unsigned int number)
	{
		unsigned int i;
		unsigned char flag;
		flag = 0;
		number = number % 10000;
		i = number/1000;
		flag = GLCD_1DigitDecimal(i, flag);			// 10^3
		number = number % 1000;
		i = number/100;
		flag = GLCD_1DigitDecimal(i, flag);			// 10^2
		number = number % 100;
		i = number/10;
		flag = GLCD_1DigitDecimal(i, flag);			// 10^1
		i = number % 10;
		lcd_char(i + '0');							// 10^0
	}

	/*-------------------------------------------------------------------------*/
