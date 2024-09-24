/*
 * gpio_interrupt.h
 *
 *  Created on: Sep 24, 2024
 *      Author: ahmad
 */

#ifndef GPIO_INTERRUPT_H_
#define GPIO_INTERRUPT_H_

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

#include "pin/pin.h"
#include "delay/delay.h"

// Prototypes for the NVIC driver
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"

// ================================= Macro ============================================
#define DEBOUNCE_DELAY      50      // 50ms

// ================================= variable =========================================
volatile pin_t button, led;

// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief PortF Interrupt Configuration
 * @param
 * @return
 * *************************************************************************************************
 */
void PortF_INT_Config(void);
/**
 * *************************************************************************************************
 * @brief Button Interrupt Handler (ISR)
 * @param
 * @return
 * *************************************************************************************************
 */
void PortF_INT_Handler(void);



#endif /* GPIO_INTERRUPT_H_ */
