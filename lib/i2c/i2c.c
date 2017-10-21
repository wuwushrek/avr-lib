#include <avr/io.h>
#include <util/twi.h>
#include "i2c.h"


void i2c_init(void){
	TWSR &= ~ (_BV(TWPS1) | _BV(TWPS0));		// No prescaler => prescaler = 1
	TWBR = ( (F_CPU / F_I2C) - 16 ) / 2;		// Bit Rate Formula
}

uint8_t i2c_start(uint8_t dev_addr){
	// Send Start condition
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

	// Wait for TWINT Flag set => START condition has been transmitted
	loop_until_bit_is_set( TWCR, TWINT);

	// Check value of the status register. Mask Prescaler bit
	if ( (TW_STATUS & 0xF8) != TW_START){
		return 1;
	}

	// load slave address into the register and read/write operation
	TWDR = dev_addr;

	// Clear TWINT to start the transmission
	TWCR = _BV(TWINT) | _BV(TWEN);

	// Wait for TWINT flag to be set
	loop_until_bit_is_set( TWCR , TWINT);

	// Check value of the status register. Mask prescaler bit
	// To verify if the device has acknowledge the READ/WRITE mode
	uint8_t twi_status = TW_STATUS & 0xF8;
	if ( (twi_status != TW_MT_SLA_ACK) && (twi_status != TW_MR_SLA_ACK) ){
		return 1;
	}

	return 0;
}

void i2c_start_wait(uint8_t dev_addr){
	uint8_t twi_status ;

	while(1){
		TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
		loop_until_bit_is_set (TWCR , TWINT);
		if ( (TW_STATUS & 0xF8) != TW_START ){
			continue;
		}
		TWDR = dev_addr;
		TWCR = _BV(TWINT) | _BV(TWEN);
		loop_until_bit_is_set( TWCR , TWINT);
		twi_status = TW_STATUS & 0xF8;
		if ( (twi_status == TW_MT_SLA_NACK) || (twi_status == TW_MR_DATA_NACK) ){
			// Device busy, send stop condition to terminate write operation
			TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);
			// Wait until stop condition is executed
			loop_until_bit_is_clear(TWCR , TWSTO);
			continue;
		}
		break;
	}
}

void i2c_stop(void){
	// Send STOP condition
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);

	// Loop until TWSTO is cleared
	loop_until_bit_is_clear(TWCR , TWSTO);
}

uint8_t i2c_write(uint8_t data){
	// Load data in the register
	TWDR = data;

	//Start sending the data
	TWCR = _BV(TWINT) | _BV(TWEN);

	//Wait for TWINT to be set
	loop_until_bit_is_set(TWCR , TWINT);

	//Check value of status register . Mask prescaler bits
	if ( (TW_STATUS & 0xF8) != TW_MT_DATA_ACK ){
		return 1;
	}

	return 0;
}

uint8_t i2c_readAck(void){
	TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	loop_until_bit_is_set (TWCR , TWINT);
	return TWDR;
}

uint8_t i2c_readNack(void){
	TWCR = _BV(TWINT) | _BV(TWEN);
	loop_until_bit_is_set(TWCR , TWINT);
	return TWDR;
}

uint8_t i2c_receive(uint8_t dev_addr, uint16_t data_size, uint8_t *data){
	if ( i2c_start( dev_addr | TW_READ)){
		return 1;
	}
	uint16_t counter;
	for (counter = 0 ; counter < (data_size - 1) ; counter++){
		data[counter] = i2c_readAck();
	}
	data[data_size-1] = i2c_readNack();

	i2c_stop();
	
	return 0;
}

uint8_t i2c_transmit(uint8_t dev_addr, uint16_t data_size, uint8_t  *data){
	if ( i2c_start ( dev_addr | TW_WRITE )){
		return 1;
	}
	uint16_t counter;
	for (counter = 0; counter < data_size ; counter++){
		if (i2c_write(data[counter]) ){
			return 1;
		}
	}
	i2c_stop();
	return 0;
}

uint8_t i2c_writeReg(uint8_t dev_addr, uint8_t reg_addr, uint16_t data_size, uint8_t *data){
	if ( i2c_start(dev_addr) ){
		return 1;
	}
	i2c_write(reg_addr);
	uint16_t counter;
	for (counter=0; counter < data_size ; counter++){
		if (i2c_write(data[counter])){
			return 1;
		}
	}

	i2c_stop();

	return 0;
}

uint8_t i2c_readReg(uint8_t dev_addr, uint8_t reg_addr, uint16_t data_size, uint8_t *data){
	if ( i2c_start(dev_addr) ){
		return 1;
	}
	i2c_write(reg_addr);
	if ( i2c_start(dev_addr | TW_READ)){
		return 1;
	}
	uint16_t counter;
	for (counter =0 ; counter < data_size-1 ; counter++){
		data[counter] = i2c_readAck();
	}
	data[data_size-1] = i2c_readNack();

	i2c_stop();

	return 0;
}