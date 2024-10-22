/*
 * Mot.h
 *
 *  Created on: Oct 22, 2024
 *      Author: ahmad
 */

#ifndef MOT_H_
#define MOT_H_

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
// ================================= variable =========================================
volatile pin_t SOL0, SOL1;
static uint8_t TMR_1MS_PULSE = 0;             // Solenoid Pulse period counter
// ================================= Macro ===========================================
#define PULSE_WIDTH         10000   //ms. Pulse with to motor
#define UNLOCK_PULSE()  {Pin_Write(SOL0, HIGH); Pin_Write(SOL1, LOW);}
#define LOCK_PULSE()    {Pin_Write(SOL0, LOW); Pin_Write(SOL1, HIGH);}
#define STOP_PULSE()    {Pin_Write(SOL0, LOW); Pin_Write(SOL1, LOW);}
// ================================= Enum ============================================
typedef enum
{
    MOT_POS_UNDEFINED,
    MOT_POS_UNLOCKED,
    MOT_POS_LOCKED,
} MOT_POSITIONS;

// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void motor_init(void);
void Unlock(void);
void Relock(void);
#endif /* MOT_H_ */
