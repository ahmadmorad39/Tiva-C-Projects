/*
 * Ultrasonic_sensor.h
 *
 *  Created on: Oct 24, 2024
 *      Author: ahmad
 */

#ifndef ULTRASONIC_SENSOR_H_
#define ULTRASONIC_SENSOR_H_


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
#include "driverlib/pwm.h"

#include "pin/pin.h"
#include "delay/delay.h"
#include "inc/tm4c123gh6pm.h"

// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
uint32_t Measure_distance(void);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void Timer0ACapture_init(void);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void Delay_MicroSecond(int time);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void Delay(unsigned long counter);


#endif /* ULTRASONIC_SENSOR_H_ */
