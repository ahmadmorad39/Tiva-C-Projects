/*
 * systick_timer.h
 *
 *  Created on: Sep 24, 2024
 *      Author: ahmad
 */

#ifndef SYSTICK_TIMER_H_
#define SYSTICK_TIMER_H_

// ================================= Header files ====================================
// Include my libraries
#include "pin/pin.h"

// Prototypes for the NVIC driver
#include "inc/hw_types.h"       // Note: Include hw_types.h before including interrupt.h
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"        // For interrupt assignments (vector numbers)

// Prototypes for the SysTick driver
#include "driverlib/systick.h"
#include "inc/hw_nvic.h"        // For SysTick Current Value Register (NVIC_ST_CURRENT)

// ================================= Variables ========================================
volatile pin_t led;

// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void Systick_INT_Init(void);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void Systick_INT_Handler(void);


#endif /* SYSTICK_TIMER_H_ */
