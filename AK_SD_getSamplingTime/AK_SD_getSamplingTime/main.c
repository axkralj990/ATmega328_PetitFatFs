/*
 * AK_SD_getSamplingTime.c
 *
 * Created: 3/20/2018 9:07:55 PM
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
	
	_delay_ms(1000);
	
	FATFS fs;
	FRESULT mountResult, openFileResult = 1, writeToFileResult;
	
	uint32_t milisecs;
	char byteString[16];
	char outBuff[1] = {0x37};
	
	/* fill the buffer with number 7 ASCII characters */
	//for(uint16_t i=0; i<512; i++){
	//	outBuff[i] = 0x37;
	//}
	
	/* set the clock to 16MHz */
	clock_prescale_set(clock_div_1);
	
	initUSART();
	init_tictoc();
	
	mountResult = pf_mount(&fs);
	if (mountResult != 0) {
		printLine("Error mounting the SD card");
		while(1);
	}
	
	UINT bytesWritten = 0;
	
	while (openFileResult) {
		openFileResult = pf_open("MYLOG.TXT");
	}
	printLine("MYLOG.TXT opened");
	
	/* write the buffer 1000 times */
	for(uint16_t i=0; i<30000; i++) {
		tic();
		/* write the 512 bytes */
		writeToFileResult = pf_write(&outBuff, 1, &bytesWritten);
		if (writeToFileResult != 0) {
			printLine("Error writing to file.");
		}
		milisecs = millis();
		ltoa(milisecs, byteString, 10);
		printLine(byteString);
	}
	pf_write(0, 0,&bytesWritten);
		
    while (1) 
    {
		printLine("main loop");
		_delay_ms(1000);
    }
}



