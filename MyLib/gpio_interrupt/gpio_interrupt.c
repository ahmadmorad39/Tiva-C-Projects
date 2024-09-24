/*
 * gpio_interrupt.c
 *
 *  Created on: Sep 24, 2024
 *      Author: ahmad
 */


// ================================= Header files ====================================
// Include my libraries
#include "gpio_interrupt/gpio_interrupt.h"

// ================================= Implementation ====================================================
// -----------------------------------------------------------------------------------------------------
extern void PortF_INT_Config(void){

    // 1) Disables the processor interrupt
    IntMasterDisable();

    // 2) Clears the portF interrupt sources
    GPIOIntClear(button.port, button.place);

    // 3) Registers an interrupt handler
    IntRegister(INT_GPIOF, PortF_INT_Handler);          // interrupt assignment = 46 --> PortF

    // 4) Sets the interrupt type
    GPIOIntTypeSet(button.port, button.place, GPIO_FALLING_EDGE);

    // 5) Sets the priority of an interrupt
    IntPrioritySet(INT_GPIOF, 0x00);

    // 6) Enables Interrupt on GPIO Level
    GPIOIntEnable(button.port, button.place);

    // 7) Enables Interrupt on NVIC level
    IntEnable(INT_GPIOF);

    // 8) Enables the processor interrupt.
    IntMasterEnable();
}
// -----------------------------------------------------------------------------------------------------
extern void PortF_INT_Handler(void){

    uint8_t int_status = 0;

    // Determines the pin that causes the interrupt
    int_status = GPIOIntStatus(button.port, true);

    if (int_status == button.place){

        // Clears the PortF interrupt sources
        GPIOIntClear(button.port, button.place);

        Pin_Write(led, TOGGLE);
        DelayMs(DEBOUNCE_DELAY);
    }
}



