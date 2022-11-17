
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "_main.h"
#include "_timer2.h"

// T/C 2 variables
unsigned int Count_Of_Timer2 = 0;	// Counter
unsigned int Task1_Of_Timer2 = 0;	// Task
unsigned int Time_Of_Timer2 = 500;	// Actual timer time

// TIMER2 initialize - prescale:64
// WGM: Normal
// desired value: 1mSec
// actual value:  0.998mSec (0.2%)  0.998246
/* Registers for Timer 2:
   TCCR2: FOC2 WGM20 COM21 COM20 WGM21 CS22 CS21 CS20
   TIMSK: OCIE2 TOIE2 TICIE1 OCIE1A OCIE1B TOIE1 OCIE0 TOIE0
   TCNT2
*/

void Timer2_init(void)		// T/C 2 initialization
{
	TCCR2 = 0x00;			// stop    
	TCNT2 = 0x1A;			// setup   
	TCCR2 = 0x03;			// start, Fosc/64
	TIMSK = 0x40;           // enable TOV2
}

SIGNAL(TIMER2_OVF_vect)
{
	TCNT2 = 0x1A;                           // From 26 to 256, 	230 X 4 times
	// setup (4.3402uS)
	Count_Of_Timer2++; 						

	if(Count_Of_Timer2 == Time_Of_Timer2)   // 1ms×500 = 500ms
	{
		Task1_Of_Timer2 = 1;				// Task1_Of_Timer2=1 at every 500ms
		Count_Of_Timer2 = 0;				// Initialize counter
	}
}