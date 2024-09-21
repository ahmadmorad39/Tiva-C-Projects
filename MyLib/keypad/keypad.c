/*
 * keypad.c
 *
 *  Created on: Sep 21, 2024
 *      Author: ahmad
 */


// ================================= Header files ====================================
/* Include this library header file */
#include "keypad.h"

// ================================= Macro ===========================================
#define KEYPAD_COLS     0x0F        // First nibble
#define KEYPAD_ROWS     0xF0        // Second nibble

// ================================= Implementation ==================================
/**
 * *************************************************************************************************
 * @brief   Initializes port working with keypad.
 * @param   None.
 * @return  None.
 * *************************************************************************************************
 */
extern void Keypad_Init(void){

    // Enable access to GPIO peripheral via the AHB.
    // --------------------------------------------
    SysCtlGPIOAHBEnable(KEYBAD_PORT_ENABLE);

    // Enable the ports.
    // ----------------
    SysCtlPeripheralEnable(KEYBAD_PORT_ENABLE);

    // Check if the peripherals access is enabled.
    // ------------------------------------------
    while(!SysCtlPeripheralReady(KEYBAD_PORT_ENABLE)){/* Nothing*/}

    // Specify the drive strength.
    // Set the first nibble for push-pull with pull-down and the second nibble for
    // Push-pull.
    // ---------------------------------------------------------------------------
    GPIOPadConfigSet(KEYPAD_PORT, KEYPAD_COLS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);   // Cols
    GPIOPadConfigSet(KEYPAD_PORT, KEYPAD_ROWS, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

    // Set the direction of all pins as input.
    // --------------------------------------
    GPIODirModeSet(KEYPAD_PORT, 0xFF, GPIO_DIR_MODE_IN);
}
/**
 * *************************************************************************************************
 * @brief     The function waits until any key is pressed and released.
 *            When released, the function returns 1 to 16, depending on the key.
 * @param     None.
 * @return    A number from 1 to 16.
 * *************************************************************************************************
 */
extern uint8_t Keypad_Click(void){

    uint8_t col = 0, row = 0, key = 0, temp = 0;

    const uint8_t keys[4][4] = { { 1,  2,  3,  4},
                                 { 5,  6,  7,  8},
                                 { 9, 10, 11, 12},
                                 {13, 14, 15, 16}
                                };
FALSE_CLICK:

    // 1) Wait unitl the keypad is clear (due to an earlier key pressed).
    // =================================================================
    // Set cols as i/p
    GPIODirModeSet(KEYPAD_PORT, KEYPAD_COLS, GPIO_DIR_MODE_IN);

    // Set rows as o/p.
    GPIODirModeSet(KEYPAD_PORT, KEYPAD_ROWS, GPIO_DIR_MODE_IN);

    // Write logic high on all raws
    GPIOPinWrite(KEYPAD_PORT, KEYPAD_ROWS, KEYPAD_ROWS);

    DelayUs(20);        // Wait for the pins to settle

    // Wait until all columns are level 0
    do{
        temp = GPIOPinRead(KEYPAD_PORT, KEYPAD_COLS);
    }while(temp != 0);

    // 2) Debouncing on key release
    // ============================
    DelayMs(20);

    // 3) Wait until a new key is pressed.
    // ==================================
    while (1){

        // Row 0
        // -----
        row = 0;    // scan 1st row
        // 1st row = o/p; others = i/p.
        GPIODirModeSet(KEYPAD_PORT, 0x10, GPIO_DIR_MODE_OUT);
        GPIODirModeSet(KEYPAD_PORT, 0xE0, GPIO_DIR_MODE_IN);

        GPIOPinWrite(KEYPAD_PORT, 0x10, 0x10);
        DelayUs(20);    // Wait for the pins settle

        temp = GPIOPinRead(KEYPAD_PORT, KEYPAD_COLS);

        if(temp != 0){
            break;  // A key is pressed
        }

        // Row 1
        // -----
        row = 1;    // scan 2st row
        // 2st row = o/p; others = i/p.
        GPIODirModeSet(KEYPAD_PORT, 0x20, GPIO_DIR_MODE_OUT);
        GPIODirModeSet(KEYPAD_PORT, 0xD0, GPIO_DIR_MODE_IN);

        GPIOPinWrite(KEYPAD_PORT, 0x20, 0x20);
        DelayUs(20);    // Wait for the pins settle

        temp = GPIOPinRead(KEYPAD_PORT, KEYPAD_COLS);

        if(temp != 0){
            break;  // A key is pressed
        }

        // Row 2
        // -----
        row = 2;    // scan 3st row
        // 3st row = o/p; others = i/p.
        GPIODirModeSet(KEYPAD_PORT, 0x40, GPIO_DIR_MODE_OUT);
        GPIODirModeSet(KEYPAD_PORT, 0xB0, GPIO_DIR_MODE_IN);

        GPIOPinWrite(KEYPAD_PORT, 0x40, 0x40);
        DelayUs(20);    // Wait for the pins settle

        temp = GPIOPinRead(KEYPAD_PORT, KEYPAD_COLS);

        if(temp != 0){
            break;  // A key is pressed
        }

        // Row 3
        // -----
        row = 3;    // scan 3st row
        // 3st row = o/p; others = i/p.
        GPIODirModeSet(KEYPAD_PORT, 0x80, GPIO_DIR_MODE_OUT);
        GPIODirModeSet(KEYPAD_PORT, 0x70, GPIO_DIR_MODE_IN);

        GPIOPinWrite(KEYPAD_PORT, 0x80, 0x80);
        DelayUs(20);    // Wait for the pins settle

        temp = GPIOPinRead(KEYPAD_PORT, KEYPAD_COLS);

        if(temp != 0){
            break;  // A key is pressed
        }
    }

    // 4) Debouncing on key press
    // ==========================
    DelayMs(20);

    // 5) Determine the column of the switch.
    // =====================================
    temp = GPIOPinRead(KEYPAD_PORT, KEYPAD_COLS);   // Read all columns simultaneously

    switch (temp){
        case 0B0001:
            col = 0;
            break;
        case 0B0010:
            col = 1;
            break;
        case 0B0100:
            col = 2;
            break;
        case 0B1000:
            col = 3;
            break;
        default:
            col = 4;       // false (noise)
    }

    // 6) Determine the key number
    // ==========================

    if(col == 4){   // false (noise)
        goto FALSE_CLICK;
    }

    key = keys[row][col];

    return key;

}
