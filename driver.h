/*
 * driver.h
 *
 *  	Created on: 2021. jan. 21.
 *      Author: Tamas Nemeth
 *      Description: Driver for AB08x5 Real-Time-Clock
 *             		AB0815 - SPI
 *      		AB0805 - I2C
 */

#ifndef DRIVER_H_
#define DRIVER_H_

#define kHz(x)		(x*1000)
#define MHz(x)		(x*1000000)
#define I2C_ADDRESS	(0xD2)

typedef enum{
   IFACE_RESET = -1,
   IFACE_I2C,
   IFACE_SPI,

   IFACE_NUMS
}iface_type;

//Variables
extern iface_type iface_sel;

//These defines must fit the currently used MCU
bool 	Get_SDIPin();						//Current MCU<->AB08x5 SDI digital pin read
void	Interface_selector(int8_t iface_sel);

//I2C functions
bool	Init_I2C(uint32_t speed, uint8_t address);		//I2C interface init
void	I2C_Write(uint8_t byte);				//I2C write byte
void 	I2C_WriteMulti(uint8_t* buf, uint8_t len);		//I2C write multi byte function
uint8_t	I2c_Read();						//I2C read byte
void 	I2C_ReadMulti(uint8_t* buf, uint8_t len);		//I2C read multi byte function
void 	I2C_ACK(bool rw);					//I2C acknowledge false: waiting for slave ack, true: send ack to slave

//SPI functions
bool	Init_SPI(uint32_t speed);				//SPI interface init
void	SPI_Write(uint8_t byte);				//SPI write one byte
void 	SPI_WriteMulti(uint8_t* buf, uint8_t len);		//SPI write multi byte function
void	SPI_ReadMulti(uint8_t* buf, uint8_t len);		//SPI read multi byte function
uint8_t SPI_Read();						//SPI read one byte

//RTC functions
void RTC_Init();
bool Write_reg(iface_type iface_sel, uint8_t offset, uint8_t*buf,  uint8_t len);	//Write AB08x5 register
bool Read_reg(iface_type iface_sel, uint8_t offset, uint8_t*buf,  uint8_t len);		//Read AB08x5 register

#endif /* DRIVER_H_ */
