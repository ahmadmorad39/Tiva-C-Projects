/*
 * seven_segment.h
 *
 *  Created on: Sep 20, 2024
 *      Author: ahmad
 */

#ifndef SEVEN_SEGMENT_H_
#define SEVEN_SEGMENT_H_

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

/* Include my library Delay */
#include "delay/delay.h"

/* Include this library header file */
#include "inc/tm4c123gh6pm.h"

// ================================= Macro ===========================================
#define DIGIT_PORT                      GPIO_PORTB_BASE
#define DIGIT_PINS                      0x7F
#define SYS_CTL_PERIP_PORT              SYSCTL_PERIPH_GPIOB
#define DELAY_TIME                      1000

// ================================= Const ===========================================
const uint8_t CC_CODE[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,
                           0x6D, 0x7D, 0x07, 0x7F, 0x6F};

// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief Configure a port to work with 7 segment
 * @param
 * @return
 * *************************************************************************************************
 */
void Digit_Port_Config(void);
/**
 * *************************************************************************************************
 * @brief Seven Segment Test
 * @param
 * @return
 * *************************************************************************************************
 */
void seven_segment_test(void);

#endif /* SEVEN_SEGMENT_H_ */
