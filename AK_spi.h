#include <avr/io.h>
#include "pff.h"

#define DOUBLE_SPEED_SPI 0

#define DDR_SPI DDRB
#define DD_MISO PB4
#define DD_MOSI PB3
#define DD_SCK PB5
#define DD_SS PB2

//-----------------------------------
// Initialize SPI               
//-----------------------------------
void init_spi(void) {
	/* Set MOSI and SCK output, all others input */
    DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
    /* Enable SPI, Master, set clock rate fck/4 */
	SPCR = ( 1<<SPE | 1<<MSTR );
	/* enable double speed SPI */
	#if DOUBLE_SPEED_SPI
		//SPSR = (1<<SPI2X);
	#endif
}

//-----------------------------------
// Transmit a byte to MMC via SPI               
//-----------------------------------
BYTE xmit_spi(BYTE c) {
	SPDR = c;
	while (!(SPSR & (1<<SPIF)));
	return SPDR;
}

//-----------------------------------
// Receive a byte from MMC via SPI               
//-----------------------------------
BYTE rcv_spi(void) {
	return xmit_spi(0xFF);
}