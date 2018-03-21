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

void initPeriodicSampler(void){
	/* set timer to CTC mode */
	TCCR2A |= (1<<WGM21);
	/* set OCR2A to get an interupt every 2 ms */
	OCR2A = 125;
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
	
	/* char bufferC[512];
	for (uint16_t i=0; i<512; i++){
		bufferC[i] = 77;
	} */
	
	printLine("Main Loop");
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
	timerValue = millis();
	if (buffPointer < 509) {
		bufferA[buffPointer] = (uint8_t)(timerValue>>24);
		bufferA[buffPointer+1] = (uint8_t)(timerValue>>16);
		bufferA[buffPointer+2] = (uint8_t)(timerValue>>8);
		bufferA[buffPointer+3] = (uint8_t)timerValue;
		buffPointer = buffPointer + 4;
		if (buffPointer >= 509) {
			buffPointer = 509;
			writeBufferA = 1;
		}
	} else if (buffPointer < 1018) {
		bufferB[buffPointer-509] = (uint8_t)(timerValue>>24);
		bufferB[buffPointer-508] = (uint8_t)(timerValue>>16);
		bufferB[buffPointer-507] = (uint8_t)(timerValue>>8);
		bufferB[buffPointer-506] = (uint8_t)timerValue;
		buffPointer = buffPointer + 4;
		if (buffPointer >= 1018){
			buffPointer = 0;
			writeBufferB = 1;
		}
	}
}


