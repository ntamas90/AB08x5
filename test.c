/*
 * test.c
 *
 *  	Created on: 2021. jan. 21.
 *      Author: Tamas Nemeth
 *      Description: Test for AB08x5 driver
 */
#include <unistd.h>
#include <stdio.h>
#include "driver.h"
#include <ctype.h>

//test data
bool 	sdipin	= true;
uint8_t rtc_registers[256];
uint8_t rtc_reg_init[8] = {0x90, 0x10, 0x05, 0x17, 0x22, 0x01, 0x21, 0x04}; //Registers from 0x00 - 0x07

int main(int argc, char **argv) {
  system("cls");
  fflush(stdout);

  //Driver init
  RTC_Init();

  //Test begin
  memset(rtc_registers, 0, sizeof(rtc_registers));					//Reset all register value
  memcpy((uint8_t*)&rtc_registers[0], rtc_reg_init, sizeof(rtc_reg_init));		//Fill registers with test values

  printf("\r\n****************************************** Test software for AB08x5 RTC ******************************************\r\n");

  while(1){
    fflush(stdin);
    char testnum = 0;

    printf("You can choose from test methods below:\r\n");
    printf("\t1. Date read\r\n");
    printf("\t2. Time read\r\n");
    printf("\t3. Date write\r\n");
    printf("\t4. Time write\r\n");
    printf("\t5. RAM? read\r\n");
    printf("\t6. RAM? write\r\n");
    printf("\t7. EXIT\r\n");

    printf("Select: ");
    scanf("%c", (char*)&testnum);
    printf("\r\n");

    testnum -= 0x30;

    if(testnum>0 && testnum<=7){
      switch(testnum){
	case 1:{
	  printf("Date read test running...\r\n");
	  uint8_t date[16] = {0};
	  RTC_ReadDate(date, sizeof(date));
	  printf("Actual date is (YY.MM.DD) %s\r\n", date);
	}break;

	case 2:{
	  printf("Time read test running...\r\n");
	  uint8_t time[16] = {0};
	  RTC_ReadTime(time, sizeof(time));
	  printf("Actual date is (HH.MM.SS) %s\r\n", time);
	}break;

	case 3:{
	  printf("Date write test running...\r\n");
	  char date[16];
	  printf("Please type the new date (YY.MM.DD): ");
	  scanf("%s", &date);
	  if(RTC_SetDate(date)) printf("New date is (YY.MM.DD) %s\r\n", date);
	}break;

	case 4:{
	  printf("Time write test running...\r\n");
	  char time[16];
	  printf("Please type the new time (HH:MM:SS): ");
	  scanf("%s", &time);
	  if(RTC_SetTime(time)) printf("New time is (HH:MM:SS) %s\r\n", time);
	}break;

	case 5:{
	  fflush(stdin);
	  printf("RAM read test running...\r\n");
	  char ramnum, offset[3];
	  uint8_t offs;
	  printf("RAM number: ");
	  scanf("%c", (char*)&ramnum);
	  fflush(stdin);
	  printf("\r\n");
	  printf("RAM offset(00-3F): ");
	  scanf("%c%c", (char*)&offset[0], (char*)&offset[1]);
	  fflush(stdin);
	  printf("\r\n");

	  ramnum -= 0x30;
	  offs	 = atoi(&offset[0]);
	  printf("ramnum: %d offset: %02x\r\n", ramnum, offs);

	  if(ramnum>=0 && ramnum<2){
	      printf("RAM read value: %02X\r\n", RTC_ReadRAMByte(ramnum, offs));
	  }else{
	      printf("Invalid RAM number!\r\n");
	  }
	}break;

	case 6:{
	  fflush(stdin);
	  printf("RAM write test running...\r\n");
	  char ramnum, offset[3], value[3];
	  uint8_t offs, val;
	  printf("RAM number: ");
	  scanf("%c", (char*)&ramnum);
	  fflush(stdin);
	  printf("\r\n");
	  printf("RAM offset(00-3F): ");
	  scanf("%c%c", (char*)&offset[0], (char*)&offset[1]);
	  fflush(stdin);
	  printf("\r\n");
	  printf("RAM value(00-FF): ");
	  scanf("%c%c", (char*)&value[0], (char*)&value[1]);
	  fflush(stdin);
	  printf("\r\n");

	  ramnum -= 0x30;
	  offs	 = atoi(&offset[0]);
	  val	 = atoi(&value[0]);

	  printf("ramnum: %d offset: %02x val: %02x\r\n", ramnum, offs, val);
	  if(ramnum>=0 && ramnum<2){
	      (RTC_WriteRAMByte(ramnum, offs, val) == true)?(printf("RTC RAM Write OK!\r\n")):(printf("RTC RAM Write Fail!\r\n"));
	  }else{
	      printf("Invalid RAM number!\r\n");
	  }
	}break;

	case 7:{
	  return 0;
	}break;

	default:{
	  printf("The selected test unfortunately not exist!\r\n");
	}
      }

      printf("\r\n\r\n\r\n");
    }else{
	printf("This test number is not valid!\r\n");
    }
  }
}
