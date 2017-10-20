#include <avr/io.h>
#include "spi.h"

void spi_init(	uint8_t lsbfirst,
				uint8_t master, 
				uint8_t mode,
				uint8_t clock_rate,
				uint8_t dblclkrate ){

	if (master){
		//configure outputs
		DDR_SPI |= _BV(DD_MOSI) | _BV(DD_SCK);
		//configure inputs
		DDR_SPI &= ~ _BV(DD_MISO);
		//Turn on pull up resistors
		//PORT_SPI |= _BV(DD_MISO)
	}else {
		//configure outputs
		DDR_SPI |= _BV(DD_MISO) ;
		//configure inputs
		DDR_SPI &= ~ (_BV(DD_MOSI) | _BV(DD_SCK));
		//Turn on pull up resistors
		//PORT_SPI |= _BV(DD_MISO)
	}

	//Set SPI register based on the parameter of the function
	//We do not enable interruption for SPI communication
	SPCR =	_BV(SPE)						|	//Enable SPI
			((lsbfirst & 0x01) << DORD) 	|	// set lsb/ msb ordering
			((master & 0x01) << MSTR)		|	// set master or slave mode
			((mode & 0x03) << CPHA)			|	// set the mode of transfer
			((clock_rate & 0x03) << SPR0);	|	// set clock rate

	//set double SPI speed bit
	SPSR = (dblclkrate & 0x01) << SPI2X; 
}

void spi_master_init(uint8_t clock_rate){
	spi_init( 0 , 1 , 0 , clock_rate , 0 );
}

void spi_slave_init(uint8_t clock_rate){
	spi_init( 0 , 0 , 0 , clock_rate , 0 );
}

void spi_transmit( uint8_t data){
	// Start transmission
	SPDR = data;
	// Wait for transmission to complete
	loop_until_bit_is_set(SPDR , SPIF);
}

uint8_t spi_receive(void){
	// Wait for reception complete
	loop_until_bit_is_set(SPDR , SPIF);
	return SPDR;
}