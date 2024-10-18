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
#include <stdio.h>

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"
#include "tm4c123gh6pm_startup_ccs.h"
#include "utils/uartstdio.h"

/* Include my libraries */
#include "delay/delay.h"
#include "lcd_lib/lcd.h"
#include "keypad/keypad.h"
#include "gpio_interrupt/gpio_interrupt.h"
#include "systick_timer_example/systick_timer.h"
#include "RTC/rtc.h"
#include "RFID/rfid.h"
#include "arduino_spi/arduino_spi.h"
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
/**
 * *************************************************************************************************
 * @brief Set the clock to run at 50MHz
 * @param
 * @return
 * *************************************************************************************************
 */
void Set_Clock_50(void);
/**
 * *************************************************************************************************
 * @brief Set the clock to run at 40MHz
 * @param
 * @return
 * *************************************************************************************************
 */
void Set_Clock_40(void);
// ================================= Main ==========================================================

int main(void){
    uint8_t cardstr[4];  // Adjusted to 4 bytes for UID
    uint8_t status;
    // Set the system clock to 50 MHz
    Set_Clock_50();
    uart0_init();
    //UARTStdioConfig(0, 115200, SysCtlClockGet());
    DelayMs(500);
    // RFID
    MFRC522_Init();

    //char *val = "Hello";  // Data to send via SPI
    //uint32_t led = 0;  // LED state
    //InitSPI();
    //InitGPIO();
    /*
    //uint8_t key = 0, label = '\0';
    float current_temp;
    Set_Clock_50();
    rtc_config();
    Set_Time(00, 34, 13, 5, 3, 1, 19);
    Get_Time();
    current_temp = Get_temp(); */
    //printf("Current Time: %02d:%02d:%02d\n", time.hour, time.minutes, time.seconds);
    /*
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

    // Systick Timer Example
    // Initialize
    //Pin_Init(led);
    //Systick_INT_Init();
     *


    while(1){

        __asm(" WFI ");                     // WFI stands for wait for interrupt.
                                            // This instruction allow the core to enter a low power
                                            // mode and stop executing code.
    }
    */
    //Keypad_Init();
    //seven_segment_test();
    //Lcd_Init();
    //Lcd_Cmd(LCD_SET_CURSOR_BEGINNING);        // Set cursor to beginning of first line
    //lcd_string("Hello World", 11);        // Display the letter 'a'
    while(1){
        printf("Hello, UART World!\n");
        Detect_RFID();
        // Request for card
        status = MFRC522_Request(PICC_REQIDL, cardstr);
        if (status == MI_OK) {
            // Card found
            printf("Card detected\n");
            printf("Card UID: %02X %02X %02X %02X\n", cardstr[0], cardstr[1], cardstr[2], cardstr[3]);
        } else if (status == MI_ERR) {
            //printf("Error in RFID request (MI_ERR)\n");
        } else {
            //printf("Unexpected status: %02X\n", status);
        }
        //ToggleLED(&led);  // Toggle the LED state
        //SendSPI(val);  // Send the SPI data
        SysCtlDelay(20000000);  // Add delay to control the speed of toggling and transmission

        //DelayMs(500);
        //Get_Time();
        //current_temp = Get_temp();
    }
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
    // SYSCTL_SYSDIV_2_5  : Frequency 50 MHz

    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_SYSDIV_2_5);
}

void Set_Clock_40(void){
    // Set the clock to run at 80MHz
    // -----------------------------
    // SYSCTL_OSC_MAIN    : OSC source is main osc
    // SYSCTL_XTAL_MAIN   : External crystal is 16MHz
    // SYSCTL_USE_PLL     : System clock is the PLL clock
    // SYSCTL_SYSDIV_2_5  : Frequency 50 MHz

    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_SYSDIV_6);
}

void Set_Clock_50(void){
    // Set the clock to run at 50MHz
    // -----------------------------
    // SYSCTL_OSC_MAIN    : OSC source is main osc
    // SYSCTL_XTAL_MAIN   : External crystal is 16MHz
    // SYSCTL_USE_PLL     : System clock is the PLL clock
    // SYSCTL_SYSDIV_4  : Frequency 50 MHz

    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_SYSDIV_4);
}

