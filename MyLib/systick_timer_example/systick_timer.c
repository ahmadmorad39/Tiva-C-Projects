/*
 * systick_timer.c
 *
 *  Created on: Sep 24, 2024
 *      Author: ahmad
 */



// ================================= Header files ====================================
// Include my libraries
#include "systick_timer_example/systick_timer.h"


// ================================= Implementation ====================================================
// -----------------------------------------------------------------------------------------------------
extern void Systick_INT_Init(void){

    // 1) Disables the processor interrupt
    IntMasterDisable();

    // 2) Disables the SysTick counter
    SysTickDisable();

    // 3) Sets the period of the SysTick counter.
    SysTickPeriodSet(16000000);                 // 200ms / (1 / 80MHz) = 16, 000, 000 Tick (<16, 777, 216)

    // 4) Clears current value register
    HWREG(NVIC_ST_CURRENT) = 0;                 // Any write to current value register clears it
                                                // HWREG macro included in "inc/hw_types.h"

    // 5) Registers an interrupt handler
    IntRegister(FAULT_SYSTICK, Systick_INT_Handler);

    // 6) Sets the priority of an interrupt
    IntPrioritySet(FAULT_SYSTICK, 0x00);

    // 7) Enables Interrupt on SysTick Level
    SysTickIntEnable();

    // 8) Enables the SysTick counter.
    SysTickEnable();

    // 9) Enable Interrupt on NVIC LEvel
    IntEnable(FAULT_SYSTICK);

    // 10) Enables the processor interrupt
    IntMasterEnable();
}

extern void Systick_INT_Handler(void){

    static uint8_t systick_int_counter = 0;

    /* The SysTick interrupt handler is not required to clear the SysTick interrupt source
     * because it is cleared automatically by the NVIC when the interrupt handler is called.
     */

    systick_int_counter++;

    if(systick_int_counter == 5){

        systick_int_counter = 0;

        Pin_Write(led, TOGGLE);
    }
}
