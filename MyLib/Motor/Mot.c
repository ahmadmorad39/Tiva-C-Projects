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
void Unlock(void)
    {
        UNLOCK_PULSE();

        DelayMs(PULSE_WIDTH);

        STOP_PULSE();
    }

void Relock(void)
{
    LOCK_PULSE();

    DelayMs(PULSE_WIDTH);

    STOP_PULSE();
}
void motor_init(void)
{
    // Pin Configuration
    // SOL0
    // ------
    SOL0.port     = PORT_D;
    SOL0.place    = PIN_7;                // SW1 on kit
    SOL0.dir      = PIN_IN;
    SOL0.current  = CURRENT_2MA;
    SOL0.type     = PIN_PU;

    // SOL1
    // ------
    SOL1.port     = PORT_F;
    SOL1.place    = PIN_4;                // SW1 on kit
    SOL1.dir      = PIN_IN;
    SOL1.current  = CURRENT_2MA;
    SOL1.type     = PIN_PU;

    Pin_Init(SOL0);
    Pin_Init(SOL1);
}
