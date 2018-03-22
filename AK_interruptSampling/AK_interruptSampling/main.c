/*
 * AK_interruptSampling.c
 *
 * Created: 3/21/2018 7:50:17 PM
 * Author : Aleksij
 */ 

#include <avr/io.h>
#include <avr/power.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <math.h>
#include <avr/interrupt.h>

#include "pff.h"

#include <util/delay.h>
#include "USART.h"
#include "tictoc.h"

volatile uint32_t timerValue;
volatile char bufferA[512], bufferB[512];
volatile uint16_t buffPointer = 0;
volatile uint8_t writeBufferA = 0, writeBufferB = 0;
volatile char byteString[16];

volatile uint8_t sinWave[512] = {128,129,131,132,134,135,137,138,140,142,143,145,146,148,149,151,152,154,155,157,159,160,162,163,165,166,168,169,171,
	172,173,175,176,178,179,181,182,184,185,186,188,189,190,192,193,195,196,197,198,200,201,202,204,205,206,207,209,210,211,212,213,214,216,217,218,
	219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,235,236,237,238,239,239,240,241,242,242,243,244,244,245,245,246,247,247,248,
	248,249,249,250,250,250,251,251,252,252,252,253,253,253,253,254,254,254,254,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,254,254,
	254,254,254,254,253,253,253,252,252,252,251,251,251,250,250,249,249,248,248,247,247,246,246,245,244,244,243,243,242,241,240,240,239,238,237,237,
	236,235,234,233,232,231,231,230,229,228,227,226,225,224,223,222,221,219,218,217,216,215,214,213,212,210,209,208,207,205,204,203,202,200,199,198,
	196,195,194,192,191,190,188,187,186,184,183,181,180,179,177,176,174,173,171,170,168,167,165,164,162,161,159,158,156,155,153,152,150,149,147,145,
	144,142,141,139,138,136,135,133,131,130,128,127,125,124,122,120,119,117,116,114,113,111,110,108,106,105,103,102,100,99,97,96,94,93,91,90,88,87,
	85,84,82,81,79,78,76,75,74,72,71,69,68,67,65,64,63,61,60,59,57,56,55,53,52,51,50,48,47,46,45,43,42,41,40,39,38,37,36,34,33,32,31,30,29,28,27,26,
	25,24,24,23,22,21,20,19,18,18,17,16,15,15,14,13,12,12,11,11,10,9,9,8,8,7,7,6,6,5,5,4,4,4,3,3,3,2,2,2,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,
	1,1,1,2,2,2,2,3,3,3,4,4,5,5,5,6,6,7,7,8,8,9,10,10,11,11,12,13,13,14,15,16,16,17,18,19,20,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,
	39,41,42,43,44,45,46,48,49,50,51,53,54,55,57,58,59,60,62,63,65,66,67,69,70,71,73,74,76,77,79,80,82,83,84,86,87,89,90,92,93,95,96,98,100,101,103,104,
	106,107,109,110,112,113,115,117,118,120,121,123,124,126,127};

void initPeriodicSampler(){
	/* set timer to CTC mode */
	TCCR2A |= (1<<WGM21);
	/* set OCR2A to get an interupt every 2 ms */
	/* OCR2A = 125; -> 2 ms */
    /* OCR2A = 31; -> 0.5 ms */
	OCR2A = 8;
	/* set 256 clock prescaler on timer 0 */
	TCCR2B |= (1<<CS22) | (1<<CS21);
	/* set Timer/Counter0 Overflow interrupt Enable */
	TIMSK2 |= (1<<OCIE2A);
	/* clear the counter register */
	TCNT2 = 0;
	/* global interrupt enable */
	sei();
}

int main(void)
{
	_delay_ms(1000);
	
	FATFS fs;
	FRESULT mountResult = 1, openFileResult = 1, writeToFileResult;
	UINT bytesWritten = 0;
	
	/* set the clock to 16MHz */
	clock_prescale_set(clock_div_1);
	
	initUSART();
	init_tictoc();
	
	while (mountResult) {
		mountResult = pf_mount(&fs);
		printLine("[ERROR] Mounting the SD card failed. Trying again...");
		for(uint8_t i=0; i<10; i++){
			printString(".");
			_delay_ms(100);
		}
		printLine(".");
		_delay_ms(100);
	}
	printLine("[OK] SD card mounted");
	
	while (openFileResult) {
		openFileResult = pf_open("MYLOG.TXT");
	}
	printLine("[OK] MYLOG.TXT opened");
	
	printLine("[OK] acquiring the data ");
	tic();
	initPeriodicSampler();
    while (millis()<10000)
    {
		if (writeBufferA) {
			/* printLine("[OK] writing to buffer A"); */
			writeToFileResult = pf_write(bufferA, 512, &bytesWritten);
			if (writeToFileResult != 0) {
				printLine("[ERROR] failed writing buffer A to the SD");
			}
			pf_write(0, 0,&bytesWritten);
			writeBufferA = 0;
		}
		
		if (writeBufferB) {
			/* printLine("[OK] writing to buffer B"); */
			writeToFileResult = pf_write(bufferB, 512, &bytesWritten);
			if (writeToFileResult != 0) {
				printLine("[ERROR] failed writing buffer B to the SD");
			}
		    pf_write(0, 0,&bytesWritten);
		    writeBufferB = 0;
		}
    }
	printLine("[OK] done writing");
}

ISR(TIMER2_COMPA_vect)
{
	timerValue = micros();
	if (buffPointer < 512) {
		bufferA[buffPointer] = sinWave[buffPointer];
		buffPointer = buffPointer + 1;
		if (buffPointer >= 512) {
			writeBufferA = 1;
		}
		} else if (buffPointer < 1024) {
		bufferB[buffPointer-512] = sinWave[buffPointer-512];
		buffPointer = buffPointer + 1;
		if (buffPointer >= 1024){
			buffPointer = 0;
			writeBufferB = 1;
		}
	}
}

/*
ISR(TIMER2_COMPA_vect)
{
	timerValue = micros();
	if (buffPointer < 509) {
		bufferA[buffPointer] = (uint8_t)(timerValue>>24);
		bufferA[buffPointer+1] = (uint8_t)(timerValue>>16);
		bufferA[buffPointer+2] = (uint8_t)(timerValue>>8);
		bufferA[buffPointer+3] = (uint8_t)timerValue;
		buffPointer = buffPointer + 4;
		if (buffPointer >= 512) {
			buffPointer = 1000;
			writeBufferA = 1;
		}
	} else if (buffPointer < 1509) {
		bufferB[buffPointer-1000] = (uint8_t)(timerValue>>24);
		bufferB[buffPointer-1000+1] = (uint8_t)(timerValue>>16);
		bufferB[buffPointer-1000+2] = (uint8_t)(timerValue>>8);
		bufferB[buffPointer-1000+3] = (uint8_t)timerValue;
		buffPointer = buffPointer + 4;
		if (buffPointer >= 1512){
			buffPointer = 0;
			writeBufferB = 1;
		}
	}
}
*/

