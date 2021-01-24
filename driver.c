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
       uint8_t* tmp = (uint8_t*)malloc(16);
       Read_reg(iface_sel, DAY, tmp, 3);	//Read sequence from DATE offset
       sprintf(buf, "%02x.%02x.%02x", tmp[2], tmp[1], tmp[0]);
       free(tmp);
   }
 }

 /*
 * Function description: Read RTC Date to string buffer (HH:MM:SS)
 * -----------------------------------
 * Parameters: 	uint8_t* buf
 * 		uint8_t len
 * Return value: none
 */
 void RTC_ReadTime(uint8_t* buf, uint8_t len){
   if(len>=TIME_STR_LEN){
       uint8_t* tmp = (uint8_t*)malloc(16);
       Read_reg(iface_sel, SECONDS, tmp, 3);	//Read sequence from HOURS_24 offset
       sprintf(buf, "%02x:%02x:%02x", tmp[2], tmp[1], tmp[0]);
       free(tmp);
   }
 }

 /*
 * Function description: Set RTC Actual Date (YY.MM.DD)
 * -----------------------------------
 * Parameters: 	input string
 * Return value: bool - success:true, fail:false
 */
 bool RTC_SetDate(char* str){
   if((strlen(str) == DATE_STR_LEN-1) && str[2]=='.' && str[5]=='.'){					//check input length
       uint8_t* tmp = (uint8_t*)malloc(16);				//allocate memory
       for(int i=0, j=0; i<9; i+=3, j++){				//ascii chars to BCD
	   uint8_t upper = str[i]-0x30;
	   uint8_t lower = str[i+1]-0x30;
	   if(CHK_BCD(upper) && CHK_BCD(lower)){
	       tmp[2-j] = (uint8_t)((upper<<4) + (lower & 0xF));
	   }else{
	       printf("Input is not valid number\r\n");
	       free(tmp);
	       return false;
	   }
       }

       if(CHK_YEAR(tmp[2]) && CHK_MONTH(tmp[1]) && CHK_DAY(tmp[0])){	//check day, month, year range
	   Write_reg(iface_sel, DAY, tmp, 3);
       }else{
	   printf("Not valid date!\r\n");
	   free(tmp);
	   return false;
       }

       free(tmp);
       return true;
   }else{
       printf("RTC_SetDate wrong input format!\r\n");
       return false;
   }
 }

 /*
 * Function description: Set RTC Actual Time (HH:MM:SS)
 * -----------------------------------
 * Parameters: 	input string
 * Return value: bool - success:true, fail:false
 */
 bool RTC_SetTime(char* str){
   if((strlen(str) == TIME_STR_LEN-1) && str[2]==':' && str[5]==':'){					//check input length
       uint8_t* tmp = (uint8_t*)malloc(16);				//allocate memory
       for(int i=0, j=0; i<9; i+=3, j++){				//ascii chars to BCD
	   uint8_t upper = str[i]-0x30;
	   uint8_t lower = str[i+1]-0x30;
	   if(CHK_BCD(upper) && CHK_BCD(lower)){
	       tmp[2-j] = (uint8_t)((upper<<4) + (lower & 0xF));
	   }else{
	       printf("Input is not valid number\r\n");
	       free(tmp);
	       return false;
	   }
       }

       if(CHK_HOUR(tmp[2]) && CHK_MIN(tmp[1]) && CHK_SEC(tmp[0])){	//check hour, min, sec range
	   Write_reg(iface_sel, SECONDS, tmp, 3);
       }else{
	   printf("Not valid time!\r\n");
	   free(tmp);
	   return false;
       }

       free(tmp);
       return true;
   }else{
       printf("RTC_SetTime wrong input format!\r\n");
       return false;
   }
 }

 /*
 * Function description: Read RTC RAM
 * -----------------------------------
 * Parameters: 	 bool section - 0:RAM1/1:RAM2
 * 		 uint8_t offset - byte number
 * Return value: int - read byte or -1 fail
 */
 int RTC_ReadRAMByte(bool section, uint8_t offset){
   uint8_t tmp;

   if((iface_sel == IFACE_SPI) && (section == true)){	//Not valid section!
       printf("Not valid RAM section!");
       return -1;
   }

   if(offset > 0x3F){					//Out of range!
       printf("Out of range!\r\n");
       return -1;
   }

   if(!section){
       Read_reg(iface_sel, RAM_1_LOW+offset, &tmp, 1);
   }else{
       Read_reg(iface_sel, RAM_2_LOW+offset, &tmp, 1);
   }

   return tmp;
 }

 /*
 * Function description: Read RTC RAM
 * -----------------------------------
 * Parameters: 	 bool section - 0:RAM1/1:RAM2
 * 		 uint8_t offset - byte number
 * Return value: int - read byte or -1 fail
 */
 bool RTC_WriteRAMByte(bool section, uint8_t offset, uint8_t value){
    if((iface_sel == IFACE_SPI) && (section == true)){	//Not valid section!
       printf("Not valid RAM section!");
       return false;
   }

   if(offset > 0x3F){					//Out of range!
       printf("Out of range!\r\n");
       return false;
   }

   if(!section){
       Write_reg(iface_sel, RAM_1_LOW+offset, &value, 1);
   }else{
       Write_reg(iface_sel, RAM_2_LOW+offset, &value, 1);
   }

   return true;
 }
 /****************************************** RTC main functions end ******************************************/

