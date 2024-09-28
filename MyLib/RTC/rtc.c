/*
 * rtc.c
 *  RTC Module DS3231
 *  Created on: Sep 27, 2024
 *      Author: ahmad
 */

// ================================= Header files ====================================

/* Include this library header file */
#include "RTC/rtc.h"


extern uint8_t decToBcd(int val)
{
    return (uint8_t)( (val/10*16) + (val%10));
}

extern int bcdToDec(uint8_t val)
{
    return (int)( (val/16*10) + (val%16));
}

extern void Set_Time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
    uint8_t set_time[7];
    set_time[0] = decToBcd(sec);
    set_time[1] = decToBcd(min);
    set_time[2] = decToBcd(hour);
    set_time[3] = decToBcd(dow);
    set_time[4] = decToBcd(dom);
    set_time[5] = decToBcd(month);
    set_time[6] = decToBcd(year);

    // I2C write
}

extern void Get_Time(void)
{
    uint8_t get_time[7];

    // I2c read
    time.seconds = bcdToDec(get_time[0]);
    time.minutes = bcdToDec(get_time[1]);
    time.hour = bcdToDec(get_time[2]);
    time.dayofweek = bcdToDec(get_time[3]);
    time.dayofmonth = bcdToDec(get_time[4]);
    time.month = bcdToDec(get_time[5]);
    time.year = bcdToDec(get_time[6]);
}

extern float Get_temp(void)
{
    uint8_t temp[2];

    // I2c read
    return ((temp[2]) + (temp[1]>>6)/4.0);
}

extern void force_temp_conv(void)
{
    uint8_t status = 0;
    uint8_t control = 0;

    // I2c read status register
    if (!(status&0x04))
    {
        //I2c read control register
        //I2c write
    }
}

