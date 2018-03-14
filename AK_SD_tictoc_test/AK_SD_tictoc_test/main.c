/*
 * AK_DS_test_1.c
 *
 * Created: 3/12/2018 8:46:59 PM
 * Author : Aleksij
 */ 

#include <avr/io.h>
#include <avr/power.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "pff.h"

#include <util/delay.h>
#include "USART.h"
#include "tictoc.h"

int main(void)
{
	
	_delay_ms(5000);
	
	char byteString[16];
	
	FATFS fs;
	FRESULT mountResult, openFileResult, writeToFileResult;
	
	uint32_t milisecs;
	
	clock_prescale_set(clock_div_1); // set clock to 16MHz
	initUSART();
	init_tictoc();
	
	mountResult = pf_mount(&fs);
	if (mountResult != 0) {
		printLine("Error mounting the SD card");
		while(1);
	}
	
	UINT bytesWritten = 0;
	
	_delay_ms(1000);
	
	openFileResult = 1;
	while (openFileResult) {
		openFileResult = pf_open("MYLOG.TXT");
	}
	printLine(">> MYLOG.TXT opened");
	
	/*
	uint8_t buffLength = 0;
	
	tic();
	_delay_ms(300);
	for(uint16_t i=0; i<1000; i++) {
		milisecs = millis();
		ltoa(milisecs, byteString, 10);
		buffLength = strlen(byteString);
		// Write the low byte
		writeToFileResult = pf_write(byteString, buffLength, &bytesWritten);
		if (writeToFileResult != 0) {
			printLine("Error writing to file.");
		}
		// Write the separator
		writeToFileResult = pf_write(",", 1, &bytesWritten);
		if (writeToFileResult != 0) {
			printLine("Error writing to file.");
		}
	}
	pf_write(0, 0,&bytesWritten);
	*/
		
	char outPacketH, outPacketL;
	
	tic();
	_delay_ms(300);
	for(uint16_t i=0; i<1000; i++) {
		milisecs = millis();
		outPacketH = (milisecs>>8);
		outPacketL = milisecs;
		/* Write the high byte */
		writeToFileResult = pf_write(&outPacketH, 1, &bytesWritten);
		if (writeToFileResult != 0) {
			printLine("Error writing to file.");
		}
		/* Write the low byte */
		writeToFileResult = pf_write(&outPacketL, 1, &bytesWritten);
		if (writeToFileResult != 0) {
			printLine("Error writing to file.");
		}
		/* Write the separator */
		writeToFileResult = pf_write(",", 1, &bytesWritten);
		if (writeToFileResult != 0) {
			printLine("Error writing to file.");
		}
	}
	pf_write(0, 0,&bytesWritten);
		
    while (1) 
    {
		printLine("main loop");
		tic();
		_delay_ms(1000);
		milisecs = millis();
    }
}

