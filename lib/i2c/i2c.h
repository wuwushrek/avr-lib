#ifndef _I2C_IMPL_H
#define _I2C_IMPL_H

/* Define CPU frequency in Hz if not already defined 	*/
#ifndef  F_CPU
#define  F_CPU	16000000UL
#endif

/* I2C clock in Hz										*/
#define  F_I2C	100000UL	

#include <inttypes.h>
#include <avr/io.h>
#include <util/twi.h>

/**
 * Initialize the I2C interface. Need to be called only once
 */
void i2c_init(void);

/**
 * Make the start condition + sends address and transfer direction
 * @param  dev_addr device address (7..1 bits) + transfer direction (LSB)
 * @return          1 if failed to access the device
 *                  0 if the device is accessible
 */
uint8_t i2c_start(uint8_t dev_addr);

/**
 * Make the start condition + sends address and transfer direction + waits
 * by polling until the device is ready
 * @param dev_addr device address (7..1 bits) + transfer direction (LSB)
 */
void i2c_start_wait(uint8_t dev_addr);

/**
 * Send one byte to I2C device
 * @param  data byte to send to the device
 * @return      0 if write was successful
 *              1 if write failed
 */
uint8_t i2c_write(uint8_t data);

/**
 * read one byte frim the I2C device and request more data from device by not
 * issuing a stop condition
 * @return  byte read from I2C device
 */
uint8_t i2c_readAck(void);

/**
 * read one byte from the I2C device and continue with a stop condition
 * @return  byte read from I2C device
 */
uint8_t i2c_readNack(void);

/**
 * Receives a certain amount a data from an I2C device and finish with
 * a STOP condition
 * @param  dev_addr  address of the I2C device
 * @param  data_size number of bytes to read
 * @param  data      array of byte read from the I2C device
 * @return           1 if reception of some data failed
 *                   0 if reception of all data was successful
 */
uint8_t i2c_receive(uint8_t dev_addr, uint16_t data_size, uint8_t *data);

/**
 * Transfer a certain amount of data to an I2C device and finish with
 * a STOP condition
 * @param  dev_addr  address of the I2C device
 * @param  data_size number of bytes to send
 * @param  data      array of byte to send to the I2C device
 * @return           1 if sending failed
 *                   0 if sending was successful
 */
uint8_t i2c_transmit(uint8_t dev_addr, uint16_t data_size, uint8_t  *data);

/**
 * Write a certain amount of data to a given register of an I2C device and finish 
 * with a STOP condition
 * @param  dev_addr  address of the I2C device
 * @param  reg_addr  register address of the I2C device were to start storing the data
 * @param  data_size number of bytes to write
 * @param  data      data to send to the I2C device
 * @return           1 if the operation failed
 *                   0 if the operation was successful
 */
uint8_t i2c_writeReg(uint8_t dev_addr, uint8_t reg_addr, uint16_t data_size, uint8_t *data);

/**
 * read a certain amount of data from a given register of an I2C device and finish 
 * with a STOP condition
 * @param  dev_addr  address of the I2C device
 * @param  reg_addr  register address of the I2C device were to start reading the data
 * @param  data_size number of bytes to read
 * @param  data      data read from the I2C device
 * @return           1 if the operation failed
 *                   0 if the operation was successful
 */
uint8_t i2c_readReg(uint8_t dev_addr, uint8_t reg_addr, uint16_t data_size, uint8_t *data);

/**
 * terminates the datat transfer and releases the I2C bus
 */
void i2c_stop(void);

/**
 * By default read function should be read of one byte with a STOP condition
 * to give the opportunity to get information from the other I2C devices
 */
#define i2c_read()	i2c_readNack();

#endif	//I2C_IMPL_H