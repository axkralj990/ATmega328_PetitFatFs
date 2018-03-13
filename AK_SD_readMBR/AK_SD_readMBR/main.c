/*
 * AK_DS_test_1.c
 *
 * Created: 3/12/2018 8:46:59 PM
 * Author : Aleksij
 */ 

#include <avr/io.h>
#include <avr/power.h>
#include <stdlib.h>

#include "diskio.h"

#include <util/delay.h>
#include "USART.h"

int main(void)
{
	uint8_t readSize = 66;
	char byte_string[16];
	
	DSTATUS statInit;
	
    //DRESULT resReceive;
	
	BYTE buff[readSize];
	DWORD sector = 0;
	UINT offset = 446;
	UINT count = readSize;
	initUSART();
	clock_prescale_set(clock_div_1); // set clock to 16MHz
	
	do {
		statInit = disk_initialize();
		
		if (statInit == 0x01) {
			printLine("Card not initialized");
			} else if (statInit == 0x02) {
			printLine("Card not found");
			} else {
			printLine("Card initialized");
		}
		_delay_ms(100);
	} while(statInit);
	
    while (1) 
    {
		printLine("=== MBR: ===");
		
		disk_readp(buff,sector,offset,count);
		
		for(uint8_t i=0; i<readSize; i++){
			utoa(buff[i],byte_string,16);
			printString(byte_string);
			printString(" ");
		}
		
		_delay_ms(1000);
    }
}

