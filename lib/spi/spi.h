#ifndef _SPI_IMPL_H
#define _SPI_IMPL_H

#ifndef		F_CPU
#define		F_CPU	16000000UL
#endif

#include <inttypes.h>

/**
 * Global spi_init 
 * @param lsbfirst   [description]
 * @param master     [description]
 * @param mode       [description]
 * @param clock_rate [description]
 * @param dblclkrate [description]
 */
void spi_init(	uint8_t lsbfirst,
				uint8_t master, 
				uint8_t mode,
				uint8_t clock_rate,
				uint8_t dblclkrate );

/**
 * initialize SPI as a master with a specified clock rate
 * Here we don't enable the double SPI Speed bit and
 * the data are sent MSB to LSB and the mode of transmission
 * is the default one
 * 
 * @param clock_rate the clock rate of the SPI connection
 */
void spi_master_init(uint8_t clock_rate);

/**
 * Initialize SPI as a slave with a specified clock rate
 * Here the data mode is the default one 0 and data are sent MSB to LSB.
 * @param clock_rate the clock rate of the SPI connection
 */
void spi_slave_init(uint8_t clock_rate);

/**
 * Send data over the SPI link
 * @param data Data to be send over the SPI connection
 */
void spi_transmit( uint8_t data);

/**
 * Receive data over SPI connection
 * @return  data received on the SPI link
 */
uint8_t spi_receive(void);


#endif 	//_SPI_IMPL_H