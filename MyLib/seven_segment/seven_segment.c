/*
 * seven_segment.c
 *
 *  Created on: Sep 20, 2024
 *      Author: ahmad
 */

/* Include my library Delay */
#include "seven_segment/seven_segment.h"


// ================================= Implementation ===================================

extern void Digit_Port_Config(void){

    // Enable the port.
    // ---------------
    SysCtlPeripheralEnable(SYS_CTL_PERIP_PORT);

    // Check if the peripherals access is enabled.
    // ------------------------------------------
    while(!SysCtlPeripheralReady(SYS_CTL_PERIP_PORT)) {}

    // Specify the output drive strength.
    // Set the pad for standard push-pull operations.
    GPIOPadConfigSet(DIGIT_PORT, DIGIT_PINS, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD);

    // Set the direction of all pins as output.
    // ----------------------------------------
    GPIODirModeSet(DIGIT_PORT, DIGIT_PINS, GPIO_DIR_MODE_OUT);

    // write logic low on all pins
    // ---------------------------
    GPIOPinWrite(DIGIT_PORT, DIGIT_PINS, 0x00);
}

extern void seven_segment_test(void){

    uint8_t index = 0;

    Digit_Port_Config();

    for(index = 0; index <=9; index++){
        GPIOPinWrite(DIGIT_PORT, DIGIT_PINS, CC_CODE[index]);
        DelayMs(DELAY_TIME);
    }
    GPIOPinWrite(DIGIT_PORT, DIGIT_PINS, 0x00);
}


