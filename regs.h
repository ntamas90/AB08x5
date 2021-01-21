/*
 * regs.h
 *
 *  Created on: 2021. jan. 21.
 *      Author: tamas.nemeth
 */

#ifndef REGS_H_
#define REGS_H_

#define DATE_STR_LEN	(9)	//YY.MM.DD+\0 termination
#define TIME_STR_LEN	(9)	//HH:MM:SS+\0 termination

//registers from https://abracon.com/Support/AppsManuals/Precisiontiming/AB08XX-Application-Manual.pdf
#define HUNDREDTHS	(0x00)
#define SECONDS		(0x01)
#define MINUTES		(0x02)
#define HOURS_24	(0x03)
#define DAY		(0x04)
#define MONTHS		(0x05)
#define YEARS		(0x06)
#define WEEKDAYS	(0x07)

#define RAM_1_LOW	(0x40)
#define RAM_1_HIGH	(0x7F)

#define RAM_2_LOW	(0x80)		//Only I2C mode
#define RAM_2_HIGH	(0xFF)

#endif /* REGS_H_ */
