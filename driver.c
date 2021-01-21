/*
 * driver.c
 *
 *  	Created on: 2021. jan. 21.
 *      Author: Tamas Nemeth
 *      Description: Driver for AB08x5 Real-Time-Clock
 *      		AB0815 - SPI
 *      		AB0805 - I2C
 */

#include "driver.h"
#include "test.h"

//Variables
 iface_type iface_sel = IFACE_RESET;		//Interface selection -> -1: Initial value, 0: I2C interface, 1: SPI interface


 /****************************************** Low Level Functions ******************************************/

 /*
 * Function description: Get the MCU SDI pin state
 * -----------------------------------
 * Parameters: none
 * Return value: bool - current pin digital state
 */
 bool Get_SDIPin(){
#ifdef TEST
   return sdipin;
#else
   bool result = false;
   //Change GPIO to INPUT with PU
   //Read GPIO state and write to result variable
   return result;
#endif
 }

 /*
 * Function description: I2C peripheral init
 * -----------------------------------
 * Parameters: none
 * Return value: bool - if init is success then return true
 */
 bool Init_I2C(uint32_t speed, uint8_t address){
   bool result = false;
   //this section must contain to the specific MCU init
   return result;
 }

 /*
 * Function description: SPI peripheral init
 * -----------------------------------
 * Parameters: none
 * Return value: bool - if init is success then return true
 */
 bool Init_SPI(uint32_t speed){
   bool result = false;
   //this section must contain to the specific MCU init
   return result;
 }

 /*
 * Function description: Get the MCU SDI pin state and initialize the suitable interface
 * -----------------------------------
 * Parameters: iface_type - interface type
 * Return value: none
 */
 void Interface_selector(iface_type iface_sel){
   if(iface_sel == IFACE_I2C){
       Init_I2C(kHz(100), I2C_ADDRESS);		//Init I2C 100kHz
   }else{
       Init_SPI(MHz(2));			//Init SPI 2MHz
   }
 }

 /*
 * Function description: Write the RTC register
 * -----------------------------------
 * Parameters: 	iface_type	- interface number
 * 		uint8_t		- register offset
 * 		uint8_t*	- buffer pointer
 * 		uint8_t		- buffer length
 *
 * Return value: none
 */
 bool Write_reg(iface_type iface_sel, uint8_t offset, uint8_t* buf,  uint8_t len){
#ifdef TEST
   for(int i=offset, j = 0; i<offset+len; i++, j++){
      rtc_registers[i] =  buf[j];
   }
   return true;
#else
   if(iface_sel == IFACE_I2C){

       return true;
   }else if(iface_sel == IFACE_SPI){

       return true;
   }

   return false;
#endif
 }

 /*
 * Function description: Read the RTC register
 * -----------------------------------
 * Parameters: 	iface_type	- interface number
 * 		uint8_t		- register offset
 * 		uint8_t*	- buffer pointer
 * 		uint8_t		- buffer length
 *
 * Return value: none
 */
 bool Read_reg(iface_type iface_sel, uint8_t offset, uint8_t* buf, uint8_t len){
#ifdef TEST
   printf("DIAG: read register %02X on %s bus\r\n", offset, (iface_sel==IFACE_I2C)?("I2C"):("SPI"));
   for(int i=offset, j=0; i<offset+len; i++, j++){
       buf[j] = rtc_registers[i];
   }
   return true;
#else
   if(iface_sel == IFACE_I2C){

       return true;
   }else if(iface_sel == IFACE_SPI){

       return true;
   }

   return false;
#endif
 }

 /****************************************** Low Level Functions end ******************************************/



 /****************************************** RTC main functions ******************************************/

 /*
 * Function description: Initialize the RTC module
 * -----------------------------------
 * Parameters: none
 * Return value: none
 */
 void RTC_Init(){
   //Auto RTC chip detection
   if(Get_SDIPin() == false){	//This function return with the AB08x5 SDI pin digital state.
       iface_sel = IFACE_I2C;	//The MCU change this pin to input with the internal pull-up and test it.
   }else{			//If the read value is 0 that the IC is I2C type
       iface_sel = IFACE_SPI;
   }

   Interface_selector(iface_sel);
 }

 /*
 * Function description: Read RTC Date to string buffer (YY.MM.DD)
 * -----------------------------------
 * Parameters: 	uint8_t* buf
 * 		uint8_t len
 * Return value: none
 */
 void RTC_ReadDate(uint8_t* buf, uint8_t len){
   if(len>=DATE_STR_LEN){
       uint8_t tmp[16];
       Read_reg(iface_sel, DAY, tmp, 3);	//Read sequence from DATE offset
       sprintf(buf, "%02x.%02x.%02x", tmp[2], tmp[1], tmp[0]);
   }
 }

 /*
 * Function description: Read RTC Date to string buffer (HH.MM.SS)
 * -----------------------------------
 * Parameters: 	uint8_t* buf
 * 		uint8_t len
 * Return value: none
 */
 void RTC_ReadTime(uint8_t* buf, uint8_t len){
   if(len>=TIME_STR_LEN){
       uint8_t tmp[16];
       Read_reg(iface_sel, SECONDS, tmp, 3);	//Read sequence from HOURS_24 offset
       sprintf(buf, "%02x:%02x:%02x", tmp[2], tmp[1], tmp[0]);
   }
 }

 /****************************************** RTC main functions end ******************************************/

