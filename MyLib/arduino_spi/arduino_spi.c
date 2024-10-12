/*
 * arduino_spi.c
 *
 *  Created on: Oct 12, 2024
 *      Author: ahmad
 */

// ================================= Header files ====================================
#include "arduino_spi/arduino_spi.h"
#include "driverlib/pin_map.h"
#include <stdio.h>

// ================================= Implementation ===================================
void InitSPI(void) {
    // Enable the peripheral for GPIO Port A (used for SPI)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure the pins for the SSI (SPI) function:
    // PA2 for SSI0 Clock (SSI0CLK)
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);

    // PA3 for SSI0 Frame Signal (SSI0FSS) - used for chip select
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);

    // PA5 for SSI0 Transmit (SSI0TX) - data transmission line
    GPIOPinConfigure(GPIO_PA5_SSI0TX);

    // Set pin types for SSI (PA2, PA3, and PA5) on Port A
    // Specifies that these pins will be used for SSI functionality
    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_5 | GPIO_PIN_3 | GPIO_PIN_2);

    // Enable the SSI0 peripheral (used for SPI communication)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    // Wait until the SSI0 peripheral is ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI0)) {}

    // Configure the SSI0 settings:
    // - Clock source: System clock (SysCtlClockGet())
    // - Frame format: Motorola SPI frame format (SSI_FRF_MOTO_MODE_0)
    // - Mode: Master mode (SSI_MODE_MASTER)
    // - Clock rate: 1 MHz (1000000)
    // - Data width: 8 bits per frame (8)
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8);

    // Enable the SSI0 module for operation
    SSIEnable(SSI0_BASE);
}

// Function to initialize GPIO for controlling LEDs on Port F
void InitGPIO(void) {
    // Enable GPIOF peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Set pins for output
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    // Initialize LEDs to off
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x00);

    // Delay for GPIO setup
    SysCtlDelay(20000000);
}


// Function to send data via SSI (SPI)
void SendSPI(char *data) {
    while (*data) {
        SSIDataPut(SSI0_BASE, *data);  // Send the data via SPI

        // Wait until transmission is complete
        while (SSIBusy(SSI0_BASE)) {}

        // Add a delay to ensure data is transmitted and the slave has time to process it
        //SysCtlDelay(20000000);  // Adjust this delay as needed

        data++;  // Move to next character
    }
}

// Function to toggle the LED state
void ToggleLED(uint32_t *led) {
    // Toggle LED connected to PF1 (Red LED)
    *led ^= 0x02;
    // Write the new LED state
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, *led);
}
