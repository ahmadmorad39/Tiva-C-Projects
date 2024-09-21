/*
 * keypad.h
 *
 *  Created on: Sep 21, 2024
 *      Author: ahmad
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

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
// ================================= Macro ===========================================
// Keypad connection pins
// -------------------
#define KEYPAD_PORT             GPIO_PORTB_AHB_BASE     // GPIO Port B (high speed)
#define KEYBAD_PORT_ENABLE      SYSCTL_PERIPH_GPIOB

// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief   Initializes port working with keypad.
 * @param   None.
 * @return  None.
 * *************************************************************************************************
 */
void Keypad_Init(void);

/**
 * *************************************************************************************************
 * @brief     The function waits until any key is pressed and released.
 *            When released, the function returns 1 to 16, depending on the key.
 * @param     None.
 * @return    A number from 1 to 16.
 * *************************************************************************************************
 */
uint8_t Keypad_Click(void);

#endif /* KEYPAD_H_ */
