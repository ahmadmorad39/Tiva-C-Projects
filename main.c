/*
 * lcd_using_lib.c
 *
 *  Created on: Sep 18, 2024
 *      Author: ahmad
 */


// ================================= Header files ====================================
/* Defines integer and  boolean data types */
#include <stdint.h>
#include <stdbool.h>

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"

/* Include my libraries */
#include "delay/delay.h"
#include "lcd_lib/lcd.h"
#include "keypad/keypad.h"
#include "gpio_interrupt/gpio_interrupt.h"

// ================================= Const ===========================================
const uint8_t Key_Label[16] = {'1', '2', '3', 'A',
                             '4', '5', '6', 'B',
                             '7', '8', '9', 'C',
                             '*', '0', '#', 'D'};

// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief Set the clock to run at 80MHz
 * @param
 * @return
 * *************************************************************************************************
 */
void Set_Clock_80(void);

// ================================= Main ==========================================================

int main(void){

    //uint8_t key = 0, label = '\0';

    Set_Clock_80();
    // Button
    // ------
    button.port     = PORT_F;
    button.place    = PIN_4;                // SW1 on kit
    button.dir      = PIN_IN;
    button.current  = CURRENT_2MA;
    button.type     = PIN_PU;

    // Green LED
    // ---------
    led.port        = PORT_F;
    led.place       = PIN_3;                // Green LED
    led.dir         = PIN_OUT;
    led.current     = CURRENT_2MA;
    led.type        = PIN_STD;

    // Initialize
    Pin_Init(button);
    Pin_Init(led);
    PortF_INT_Config();

    while(1){

        __asm(" WFI ");                     // WFI stands for wait for interrupt.
                                            // This instruction allow the core to enter a low power
                                            // mode and stop executing code.
    }

    //Keypad_Init();
    //seven_segment_test();
    //Lcd_Init();
    //Lcd_Cmd(LCD_SET_CURSOR_BEGINNING);        // Set cursor to beginning of first line
    //lcd_string("Hello World", 11);        // Display the letter 'a'
    //while(1){
    //    key = Keypad_Click();
    //    label = Key_Label[key - 1];
    //}
    return 0;
}


void Set_Clock_80(void){
    // Set the clock to run at 80MHz
    // -----------------------------
    // SYSCTL_OSC_MAIN    : OSC source is main osc
    // SYSCTL_XTAL_MAIN   : External crystal is 16MHz
    // SYSCTL_USE_PLL     : System clock is the PLL clock
    // SYSCTL_SYSDIV_2_5  : Frequency 80 MHz

    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_SYSDIV_2_5);
}


