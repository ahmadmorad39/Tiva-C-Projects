/*
 * arduino_spi.h
 *
 *  Created on: Oct 12, 2024
 *      Author: ahmad
 */

#ifndef ARDUINO_SPI_H_
#define ARDUINO_SPI_H_

// ================================= Header files ====================================
// Defines integer data and boolean types
#include <stdint.h>
#include <stdbool.h>

/* Macros defining the memory map of the device */
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/debug.h"

/* Defines and Prototypes for GPIO API */
#include "driverlib/gpio.h"
#include "driverlib/ssi.h"


// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void InitSPI(void);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void InitGPIO(void);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void SendSPI(char *data);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void ToggleLED(uint32_t *led);
#endif /* ARDUINO_SPI_H_ */
