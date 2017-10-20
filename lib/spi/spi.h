#ifndef _SPI_IMPL_H
#define _SPI_IMPL_H

#ifndef		F_CPU
#define		F_CPU	16000000UL
#endif

/**
 * MOSI , MISO , SCK and SS PORT can be changed using this macros in
 * the Makefile
 */
#ifndef
#define DDR_SPI		DDRB
#endif

#ifndef
#define PORT_SPI	PORTB	
#endif

#ifndef
#define DD_MOSI		DDB3
#endif

#ifndef
#define DD_SCK		DDB5
#endif

#ifndef
#define DD_MISO		DDB4
#endif

#ifndef
#define DD_SS		DDB2
#endif

#include <inttypes.h>

/**
 * Global SPI bus init
 * @param lsbfirst   if 0: most significant bit is transmitted first
 * @param master     if 1: initiatization as master else as slave
 * @param mode       The 4 data transfer modes according to the datasheet
 * @param clock_rate value between 0-3 ! It helps dividing clock freq by 4^(clock_rate+1)
 * @param dblclkrate if 1: double the frequency of the SPI communication
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