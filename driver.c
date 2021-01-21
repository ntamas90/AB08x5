/*
 * driver.c
 *
 *  	Created on: 2021. jan. 21.
 *      Author: Tamas Nemeth
 *      Description: Driver for AB08x5 Real-Time-Clock
 *      		AB0815 - SPI
 *      		AB0805 - I2C
 */

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "driver.h"

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
   bool result = false;
   //Change GPIO to INPUT with PU
   //Read GPIO state and write to result variable
   return result;
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
 * Parameters: int8_t - interface number
 * Return value: none
 */
 void Interface_selector(int8_t iface_sel){
   if(iface_sel == IFACE_I2C){
       Init_I2C(kHz(100), I2C_ADDRESS);		//Init I2C 100kHz
   }else{
       Init_SPI(MHz(2));			//Init SPI 2MHz
   }
 }

 /****************************************** Low Level Functions end ******************************************/

 /*
 * Function description: Initialize the RTC module
 * -----------------------------------
 * Parameters: none
 * Return value: none
 */
 void RTC_Init(){
   //Auto RTC chip detection
   if(Get_SDIPin() == false){	//This function return with the AB08x5 SDI pin digital state.
       iface_sel = 0;		//The MCU change this pin to input with the internal pull-up and test it.
   }else{
       iface_sel = 1;
   }

   Interface_selector(iface_sel);
 }
