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

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include "regs.h"

#define kHz(x)		(x*1000)
#define MHz(x)		(x*1000000)
#define I2C_ADDRESS	(0xD2)

#define CHK_BCD(x)	((x>=0x00 && x<=0x09)?(true):(false))
#define CHK_YEAR(x)	((x>=0x00 && x<=0x99)?(true):(false))
#define CHK_MONTH(x)	((x>=0x01 && x<=0x12)?(true):(false))
#define CHK_DAY(x)	((x>=0x01 && x<=0x31)?(true):(false))
#define CHK_HOUR(x)	((x>=0x00 && x<=0x23)?(true):(false))
#define CHK_MIN(x)	((x>=0x00 && x<=0x59)?(true):(false))
#define CHK_SEC(x)	((x>=0x00 && x<=0x59)?(true):(false))

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
void	Interface_selector(iface_type iface_sel);

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
void 	RTC_Init();
bool 	Write_reg(iface_type iface_sel, uint8_t offset, uint8_t*buf,  uint8_t len);		//Write AB08x5 register
bool 	Read_reg(iface_type iface_sel, uint8_t offset, uint8_t*buf,  uint8_t len);		//Read AB08x5 register
void 	RTC_ReadDate(uint8_t* buf, uint8_t len);						//Read actual date in YY.MM.DD format
void 	RTC_ReadTime(uint8_t* buf, uint8_t len);						//Read actual time in HH.MM.SS format
bool 	RTC_SetDate(char* str);									//Write actual date in YY.MM.DD format
bool 	RTC_SetTime(char* str);									//Write actual time in HH.MM.SS format
int 	RTC_ReadRAMByte(bool section, uint8_t offset);
bool	RTC_WriteRAMByte(bool section, uint8_t offset, uint8_t value);

#endif /* DRIVER_H_ */
