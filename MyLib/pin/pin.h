/*
 * pin.h
 *
 *  Created on: Sep 21, 2024
 *      Author: ahmad
 */

#ifndef PIN_H_
#define PIN_H_

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

// ================================= Macro ===========================================

// Ports
// -----
#define PORT_A          GPIO_PORTA_AHB_BASE
#define PORT_B          GPIO_PORTB_AHB_BASE
#define PORT_C          GPIO_PORTC_AHB_BASE
#define PORT_D          GPIO_PORTD_AHB_BASE
#define PORT_E          GPIO_PORTE_AHB_BASE
#define PORT_F          GPIO_PORTF_AHB_BASE

// Pin
// ---
#define PIN_0           GPIO_PIN_0
#define PIN_1           GPIO_PIN_1
#define PIN_2           GPIO_PIN_2
#define PIN_3           GPIO_PIN_3
#define PIN_4           GPIO_PIN_4
#define PIN_5           GPIO_PIN_5
#define PIN_6           GPIO_PIN_6
#define PIN_7           GPIO_PIN_7

// Pad type
// --------
#define PIN_STD         GPIO_PIN_TYPE_STD       // Push-pull
#define PIN_PU          GPIO_PIN_TYPE_STD_WPU   // Push-pull with weak pull-up
#define PIN_PD          GPIO_PIN_TYPE_STD_WPD   // Push-pull with weak pull-down

// Current
// -------
#define CURRENT_2MA     GPIO_STRENGTH_2MA
#define CURRENT_4MA     GPIO_STRENGTH_4MA
#define CURRENT_8MA     GPIO_STRENGTH_8MA

// Available PIN Commands
// ----------------------
//      *** IMPORTANT: DO NOT CHANGE THESE VALUES ***
// --------------------------------------------------
#define LOW             0                       // Turn PIN off
#define HIGH            1                       // Turn PIN on
#define TOGGLE          2                       // Invert the PIN


// ================================= Structure ===========================================
// To store a PIN properties
typedef struct {
    uint32_t port;          // Port address
    uint8_t  place;         // Pin Place
    uint32_t dir;           // Pin direction
    uint32_t current;       // Pin maximum current
    uint32_t type;          // Pad type
} pin_t;                    // Pin data type

// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void Pin_Init(pin_t pin);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void Pin_write(pin_t pin, uint8_t cmd);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
int8_t Pin_Read(pin_t pin);

#endif /* PIN_H_ */
