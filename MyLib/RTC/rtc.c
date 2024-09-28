/*
 * rtc.c
 *  RTC Module DS3231
 *  Created on: Sep 27, 2024
 *      Author: ahmad
 */

// ================================= Header files ====================================
#include "RTC/rtc.h"
#include "driverlib/pin_map.h"
#include <stdio.h>

// ================================= Macro ===========================================
#define I2C_TIMEOUT 10000

// ================================= Implementation ===================================
// Helper function to wait for I2C transaction completion and check errors
static bool I2CWaitForCompletion(uint32_t i2c_base)
{
    uint32_t timeout = 0;

    while (I2CMasterBusy(i2c_base)) {
        if (timeout++ > I2C_TIMEOUT) {
            // I2C Timeout Error
            return false; // Timeout error
        }
    }

    // Check for specific I2C errors
    uint32_t error_code = I2CMasterErr(i2c_base);
    if (error_code != I2C_MASTER_ERR_NONE) {
        switch (error_code) {
            case I2C_MASTER_ERR_ARB_LOST:
                // I2C Error: Arbitration lost
                break;
            case I2C_MASTER_ERR_ADDR_ACK:
                // I2C Error: Address not acknowledged
                break;
            case I2C_MASTER_ERR_DATA_ACK:
                // I2C Error: Data not acknowledged
                break;
            case I2C_MASTER_ERR_CLK_TOUT:
                // I2C Error: Clock timeout
                break;
            default:
                // I2C Error: Unknown (Code: 0x%x)\n", error_code
        }
        return false; // I2C error
    }

    return true;
}

// ----------------------------------------------------------------------------------------
static uint8_t decToBcd(int val)
{
    return (uint8_t)((val / 10 * 16) + (val % 10));
}
// ----------------------------------------------------------------------------------------
static int bcdToDec(uint8_t val)
{
    return (int)((val / 16 * 10) + (val % 16));
}
// ----------------------------------------------------------------------------------------
void Set_Time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
    uint8_t set_time[7];
    set_time[0] = decToBcd(sec);
    set_time[1] = decToBcd(min);
    set_time[2] = decToBcd(hour);
    set_time[3] = decToBcd(dow);
    set_time[4] = decToBcd(dom);
    set_time[5] = decToBcd(month);
    set_time[6] = decToBcd(year);

    // Start communication with the RTC
    I2CMasterSlaveAddrSet(I2C1_BASE, RTC_ADDR, false); // Write mode
    I2CMasterDataPut(I2C1_BASE, RTCMD_DateTime);       // Set the register to start at
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    if (!I2CWaitForCompletion(I2C1_BASE)) return;
    int i;
    for (i = 0; i < 7; i++) {
        I2CMasterDataPut(I2C1_BASE, set_time[i]);      // Send time data
        if (i < 6) {
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
        } else {
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH); // Finish the transmission
        }
        if (!I2CWaitForCompletion(I2C1_BASE)) return;
    }
}
// ----------------------------------------------------------------------------------------
void Get_Time(void)
{
    uint8_t get_time[7];

    // Set the register to start reading from
    I2CMasterSlaveAddrSet(I2C1_BASE, RTC_ADDR, false); // Write mode
    I2CMasterDataPut(I2C1_BASE, RTCMD_DateTime);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    if (!I2CWaitForCompletion(I2C1_BASE)) return;

    // Read the data
    I2CMasterSlaveAddrSet(I2C1_BASE, RTC_ADDR, true); // Read mode
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    if (!I2CWaitForCompletion(I2C1_BASE)) return;
    int i;
    for (i = 0; i < 7; i++) {
        get_time[i] = I2CMasterDataGet(I2C1_BASE); // Receive time data
        if (i < 6) {
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        } else {
            I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
        }
        if (!I2CWaitForCompletion(I2C1_BASE)) return;
    }

    // Store the time values in the `time` struct
    time.seconds    = bcdToDec(get_time[0]);
    time.minutes    = bcdToDec(get_time[1]);
    time.hour       = bcdToDec(get_time[2]);
    time.dayofweek  = bcdToDec(get_time[3]);
    time.dayofmonth = bcdToDec(get_time[4]);
    time.month      = bcdToDec(get_time[5]);
    time.year       = bcdToDec(get_time[6]);
}
// ----------------------------------------------------------------------------------------
float Get_temp(void)
{
    uint8_t temp[2];

    // Set the register to the temperature MSB
    I2CMasterSlaveAddrSet(I2C1_BASE, RTC_ADDR, false); // Write mode
    I2CMasterDataPut(I2C1_BASE, 0x11); // Temperature MSB register address
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    if (!I2CWaitForCompletion(I2C1_BASE)) return -1; // Error

    // Read temperature data
    I2CMasterSlaveAddrSet(I2C1_BASE, RTC_ADDR, true); // Read mode
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    if (!I2CWaitForCompletion(I2C1_BASE)) return -1; // Error
    temp[0] = I2CMasterDataGet(I2C1_BASE);

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    if (!I2CWaitForCompletion(I2C1_BASE)) return -1; // Error
    temp[1] = I2CMasterDataGet(I2C1_BASE);

    // Calculate and return temperature: MSB + (LSB >> 6) * 0.25
    return (float)temp[0] + ((temp[1] >> 6) * 0.25);
}
// ----------------------------------------------------------------------------------------
void force_temp_conv(void)
{
    uint8_t control;

    // Read control register
    I2CMasterSlaveAddrSet(I2C1_BASE, RTC_ADDR, false); // Write mode
    I2CMasterDataPut(I2C1_BASE, RTCMD_Control);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    if (!I2CWaitForCompletion(I2C1_BASE)) return;

    I2CMasterSlaveAddrSet(I2C1_BASE, RTC_ADDR, true); // Read mode
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    if (!I2CWaitForCompletion(I2C1_BASE)) return;
    control = I2CMasterDataGet(I2C1_BASE);

    // Set the CONV bit if not already set
    if (!(control & 0x20)) {
        control |= 0x20;
        I2CMasterSlaveAddrSet(I2C1_BASE, RTC_ADDR, false); // Write mode
        I2CMasterDataPut(I2C1_BASE, RTCMD_Control);        // Write back to control register
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
        if (!I2CWaitForCompletion(I2C1_BASE)) return;

        I2CMasterDataPut(I2C1_BASE, control);              // Set CONV bit
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
        I2CWaitForCompletion(I2C1_BASE);
    }
}

// ----------------------------------------------------------------------------------------
void rtc_config(void)
{
    // Enable I2C1 and GPIO port A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure pins PA6 (SCL) and PA7 (SDA) for I2C operation
    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);

    // Wait until the peripheral is ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_I2C1)) {}

    // Initialize I2C1 Master to run at 100kHz (standard mode)
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), false);
}
