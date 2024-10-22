/*
 * Mot.c
 *
 *  Created on: Oct 22, 2024
 *      Author: ahmad
 */

// ================================= Header files ====================================
#include "Motor/Mot.h"
#include "driverlib/pin_map.h"
#include <stdio.h>

// ================================= Implementation ===================================
// Function to unlock the mechanism
void Unlock(void) {
    UNLOCK_PULSE();                 // Send an unlock pulse
    DelayMs(PULSE_WIDTH);          // Delay for the width of the pulse
    STOP_PULSE();                  // Stop the pulse
}

// Function to unlock using PWM
void Unlock_pwm(void) {
    PWM1_3_CMPA_R = 8000;         // Set PWM duty cycle to 50% (assuming LOAD_R = 16000)

    // Keep LED on for 5 seconds
    DelayMs(5000);                 // Delay for 5 seconds to keep LED on

    // Stop PWM output for PWM1
    PWM1_ENABLE_R &= ~(1 << 6);    // Disable PWM output for PWM1 generator 3
}

// Function to relock the mechanism
void Relock(void) {
    LOCK_PULSE();                  // Send a lock pulse
    DelayMs(PULSE_WIDTH);          // Delay for the width of the pulse
    STOP_PULSE();                  // Stop the pulse
}

// Function to initialize the motor and pins
void motor_init(void) {
    // Pin Configuration for SOL0
    SOL0.port     = PORT_D;        // Set port for SOL0
    SOL0.place    = PIN_7;         // Configure SOL0 pin (SW1 on kit)
    SOL0.dir      = PIN_IN;        // Set SOL0 as input
    SOL0.current  = CURRENT_2MA;   // Set SOL0 current
    SOL0.type     = PIN_PU;        // Set SOL0 pin type (pull-up)

    // Pin Configuration for SOL1
    SOL1.port     = PORT_F;        // Set port for SOL1
    SOL1.place    = PIN_4;         // Configure SOL1 pin (SW1 on kit)
    SOL1.dir      = PIN_IN;        // Set SOL1 as input
    SOL1.current  = CURRENT_2MA;   // Set SOL1 current
    SOL1.type     = PIN_PU;        // Set SOL1 pin type (pull-up)

    // Initialize pins
    Pin_Init(SOL0);                // Initialize SOL0 pin
    Pin_Init(SOL1);                // Initialize SOL1 pin
}

// Function to configure PWM settings
void configuration_pwm(void) {
    // Enable the PWM module
    SYSCTL_RCGCPWM_R |= (1<<1);   // Enable clock for PWM1
    SYSCTL_RCGCGPIO_R |= (1<<5);   // Enable clock for GPIO port F
    SYSCTL_RCC_R &= (~(1<<20));     // Clear the PWM divider

    // Configure GPIO for PWM function
    GPIO_PORTF_AFSEL_R |= (1<<2);  // Enable alternate function for PF2
    GPIO_PORTF_PCTL_R = 0x00000500; // Set PF2 to PWM output
    GPIO_PORTF_DEN_R |= (1<<2);     // Enable digital functionality for PF2

    // Configure PWM generator 3
    PWM1_3_CTL_R &= (~(1<<0));      // Disable PWM generator 3
    PWM1_3_CTL_R &= (~(1<<1));      // Disable PWM generator 3 for trigger mode
    PWM1_3_LOAD_R = 16000;          // Set the PWM load value (frequency)
    PWM1_3_CMPA_R = 8000;           // Set compare value for 50% duty cycle
    PWM1_3_GENA_R |= (1<<2) | (1<<3) | (1<<7); // Configure PWM output
    PWM1_3_CTL_R |= (1<<0);         // Enable PWM generator 3
    PWM1_ENABLE_R |= (1<<6);         // Enable PWM output for PWM1 generator 3
}
