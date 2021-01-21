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

//test data
bool 	sdipin	= true;
uint8_t rtc_registers[] = {0x90, 0x10, 0x05, 0x17, 0x22, 0x01, 0x21, 0x04}; //Registers from 0x00 - 0x07

int main(int argc, char **argv) {
  system("cls");
  fflush(stdout);

  //Driver init
  RTC_Init();

  //Test begin
  uint8_t testnum = 0;

  printf("\r\n****************************************** Test software for AB08x5 RTC ******************************************\r\n");

  while(1){
    printf("You can choose from test methods below:\r\n");
    printf("\t1. Test Date read\r\n");
    printf("\t2. Test Time read\r\n");
    printf("\t3. Test RAM read\r\n");
    printf("\t4. EXIT\r\n");

    printf("Select: ");
    scanf("%d", &testnum);
    printf("\r\n");

    switch(testnum){
      case 1:{
	printf("Run Date test...\r\n");
	uint8_t date[16] = {0};
	RTC_ReadDate(date, sizeof(date));
	printf("Actual date is (YY.MM.DD) %s.\r\n", date);
      }break;
      case 2:{
	printf("Run Time test...\r\n");
	uint8_t time[16] = {0};
	RTC_ReadTime(time, sizeof(time));
	printf("Actual date is (HH.MM.SS) %s.\r\n", time);
      }break;
      case 4:{
	return 0;
      }break;

      default:{
	printf("The selected test unfortunately not exist!\r\n");
      }
    }

    printf("\r\n\r\n\r\n");
  }
}
