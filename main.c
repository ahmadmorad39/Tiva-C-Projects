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

// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief Configure MCU pins to work with LCD
 * @param
 * @return
 * *************************************************************************************************
 */
void Set_Clock_80(void);


// ================================= Main ==========================================================

int main(void){

    Set_Clock_80();
    Lcd_Init();
    Lcd_Cmd(LCD_SET_CURSOR_BEGINNING);        // Set cursor to beginning of first line
    lcd_string("Hello World", 11);        // Display the letter 'a'
    while(1){
    }
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
