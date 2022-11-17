
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "_main.h"
#include "_eeprom.h"

void In_EEPROM_Write(unsigned int EE_Addr, unsigned char EE_Data)
{
	while(EECR&(1<<EEWE));
	EEAR = EE_Addr;
	EEDR = EE_Data; // EEDR <- EE_Data
	cli();// Global Interrupt Disable
	EECR |= (1 << EEMWE); // SET EEMWE
	EECR |= (1 << EEWE); // SET EEWE
	sei(); // Global Interrupt Enable
}

unsigned char In_EEPROM_Read(unsigned int EE_Addr)
{
	while(EECR&(1<<EEWE));
	EEAR = EE_Addr;
	EECR |= (1 <<EERE); // SET EERE
	return EEDR; // Return Read Value
}