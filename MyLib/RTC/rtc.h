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
#include "inc/hw_types.h"

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"

/* Defines and Prototypes for GPIO API */
#include "driverlib/gpio.h"

#include "driverlib/i2c.h"

// ================================= Macro ===========================================

#define RTC_ADDR                    0x68 // RTC I2C address
#define EEPROM_ADDR                 0x57  // EEPROM address
// MC RTC commands
#define RTCMD_DateTime              0x0 //  sec; min; hour; weekday; date; month; year


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

// Function to configure RTC
void rtc_config(void);

// Function to set time
void Set_Time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);

// Function to get time
void Get_Time(void);

// Function to get temperature
float Get_temp(void);

// Function to force temperature convertion
void force_temp_conv(void);

#endif /* RTC_H_ */
