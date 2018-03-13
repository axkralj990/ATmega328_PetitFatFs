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
#include "diskio.h"

#include <util/delay.h>
#include "USART.h"

int main(void)
{

	char byte_string[16];
	
	FATFS fs;
	FRESULT mountResult, openFileResult, readFromFileResult;
	const char path2file[] = "DATALOG.TXT";
	
	initUSART();
	clock_prescale_set(clock_div_1); // set clock to 16MHz
	
	mountResult = pf_mount(&fs);
	if (mountResult != 0) {
		printLine("Error mounting the SD card");
		while(1);
	}
	printString("Mount: ");
	utoa(mountResult, byte_string, 10);
	printLine(byte_string);
	
	openFileResult = pf_open(path2file);
	if (openFileResult != 0) {
		printLine("Error creating a new file");
		while(1);
	}
	printString("New File: ");
	utoa(openFileResult, byte_string, 10);
	printLine(byte_string);
	
	UINT bytesRead = 0;
	
	/*
	char readBuff[14];
	
	readFromFileResult = pf_read(readBuff, 14, &bytesRead);
	if (readFromFileResult != 0) {
		printLine("Error reading from file.");
		while(1);
	}
	
	printLine("Received bytes:");
	for(uint8_t i=0; i<14; i++) {
		transmitByte(readBuff[i]);
	}
	printLine("");
	printLine("Bytes read");
	utoa(bytesRead, byte_string, 10);
	printLine(byte_string);
	*/
	
	printString("File Content: ");
	do {
		readFromFileResult = pf_read(0, 512, &bytesRead);
	} while(!(readFromFileResult || bytesRead < 512));
	
	printLine("");
	printLine("EOF Reached");
	utoa(bytesRead, byte_string, 10);
	printString("Bytes read: ");
	printLine(byte_string);
	
    while (1) 
    {
		printLine("main loop");
		_delay_ms(1000);
    }
}

