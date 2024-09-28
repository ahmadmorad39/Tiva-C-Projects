/*
 * rtc.h
 *  RTC Module DS3231
 *  Created on: Sep 27, 2024
 *      Author: ahmad
 */

#ifndef RTC_H_
#define RTC_H_

// ================================= Header files ====================================
// Defines integer data and boolean types
#include <stdint.h>
#include <stdbool.h>

/* Macros defining the memory map of the device */
#include "inc/hw_memmap.h"

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"

/* Defines and Prototypes for GPIO API */
#include "driverlib/gpio.h"

#include "inc/hw_i2c.h"
#include "driverlib/i2c.h"

// ================================= Macro ===========================================

#define RTC_ADDR 0x68 // RTC I2C address
// MC RTC commands
#define RTCMD_DateTime              0x0 //  sec; min; hour; weekday; date; month; year
#define RTCMD_Timer0                0xb
#define RTCMD_Timer1                0xc
#define RTCMD_Extension             0xd
#define RTCMD_Flag                  0xe
#define RTCMD_Control               0xf


#define USEL_1MINUTE                0x20
#define USEL_1SECOND                0x00
#define UIE_ON                      0x20
#define TD_10Hz                     0x02
#define TIE_TE_ON                   0x10


typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hour;
    uint8_t dayofweek;
    uint8_t dayofmonth;
    uint8_t month;
    uint8_t year;
} TIME;

TIME time;

// Function to set time
void Set_Time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);

// Function to get time
void Get_Time(void);

// Function to get temperature
float Get_temp(void);

// Function to force temperature convertion
void force_temp_conv(void);

// Convert normal decimal numbers to binary coded decimal
uint8_t decToBcd(int val);

// Convert binary coded decimal to normal decimal numbers
int bcdToDec(uint8_t val);


#endif /* RTC_H_ */
