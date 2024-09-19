/*
 * lcd.c
 *
 *  Created on: Sep 18, 2024
 *      Author: ahmad
 */


// ================================= Header files ====================================
/* Defines integer and  boolean data types */
#include <stdint.h>
#include <stdbool.h>

/* Macros defining the memory map of the device */
#include "inc/hw_memmap.h"

/* Prototypes for the system control driver */
#include "driverlib/sysctl.h"

/* Defines and Prototypes for GPIO API */
#include "driverlib/gpio.h"

/* Include my library Delay */
#include "delay/delay.h"

/* Include this library header file */
#include "lcd_lib/lcd.h"
#include "inc/tm4c123gh6pm.h"


// ================================= Variables ====================================
static uint8_t DATA_PINS1 = LCD_D0 | LCD_D1 | LCD_D2;
static uint8_t DATA_PINS2 = LCD_D3 | LCD_D6 | LCD_D7;
static uint8_t DATA_PINS3 = LCD_D4 | LCD_D5;

// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief Configure MCU pins to work with LCD
 * @param
 * @return
 * *************************************************************************************************
 */
static void MCU_Pins_Config(void);

/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
static void Lcd_Send(unsigned char data);
static void lcd_data(unsigned char data);

// ================================= Implementation ================================================
static void MCU_Pins_Config (void){
    // Enable the ports.
    // -----------------
    SysCtlPeripheralEnable(LCD_DATA_PORT_ENABLE1);
    SysCtlPeripheralEnable(LCD_DATA_PORT_ENABLE2);
    SysCtlPeripheralEnable(LCD_DATA_PORT_ENABLE3);
    SysCtlPeripheralEnable(LCD_CONTROL_PORT_ENABLE);

    // Check if the peripherals access is enabled.
    // ------------------------------------------
    while(!SysCtlPeripheralReady(LCD_DATA_PORT_ENABLE1)) {}
    while(!SysCtlPeripheralReady(LCD_DATA_PORT_ENABLE2)) {}
    while(!SysCtlPeripheralReady(LCD_DATA_PORT_ENABLE3)) {}
    while(!SysCtlPeripheralReady(LCD_CONTROL_PORT_ENABLE)) {}

    // Specify the output drive strength.
    // Set the pad for standard push-pull operations.
    GPIOPadConfigSet(LCD_DATA_PORT1, DATA_PINS1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(LCD_DATA_PORT2, DATA_PINS2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(LCD_DATA_PORT3, DATA_PINS3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);
    GPIOPadConfigSet(LCD_CONTROL_PORT, (LCD_RS | LCD_RW | LCD_EN), GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

    // Set the direction of all pins as output.
    // ----------------------------------------
    GPIODirModeSet(LCD_DATA_PORT1, DATA_PINS1, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(LCD_DATA_PORT2, DATA_PINS2, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(LCD_DATA_PORT3, DATA_PINS3, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(LCD_CONTROL_PORT, (LCD_RS | LCD_RW | LCD_EN), GPIO_DIR_MODE_OUT);
}

// -------------------------------------------------------------------------------------------------
static void Lcd_Send(unsigned char data){

    // Set data pins D0 to D7 based on input data bits
    if (data & 0x01) { GPIO_PORTA_DATA_R |= (1<<7); } else { GPIO_PORTA_DATA_R &= ~(1<<7); }
    if (data & 0x02) { GPIO_PORTA_DATA_R |= (1<<6); } else { GPIO_PORTA_DATA_R &= ~(1<<6); }
    if (data & 0x04) { GPIO_PORTA_DATA_R |= (1<<5); } else { GPIO_PORTA_DATA_R &= ~(1<<5); }
    if (data & 0x08) { GPIO_PORTB_DATA_R |= (1<<4); } else { GPIO_PORTB_DATA_R &= ~(1<<4); }
    if (data & 0x10) { GPIO_PORTE_DATA_R |= (1<<5); } else { GPIO_PORTE_DATA_R &= ~(1<<5); }
    if (data & 0x20) { GPIO_PORTE_DATA_R |= (1<<4); } else { GPIO_PORTE_DATA_R &= ~(1<<4); }
    if (data & 0x40) { GPIO_PORTB_DATA_R |= (1<<1); } else { GPIO_PORTB_DATA_R &= ~(1<<1); }
    if (data & 0x80) { GPIO_PORTB_DATA_R |= (1<<0); } else { GPIO_PORTB_DATA_R &= ~(1<<0); }
}

extern void Lcd_Init(void){

    // Configure MCU pins to work with LCD
    MCU_Pins_Config();

    // 1) Wait more than 15 ms after power is applied
    DelayMs(20);

    // 2) 8-bit mode, 2 lines, 5x7 matrix
    Lcd_Cmd(LCD_FONT_8BIT_CONFIG);
    DelayMs(20);

    // 3) Increment cursor
    Lcd_Cmd(LCD_INCREMENT_CURSOR);

    // 4) Display ON, cursor Off
    Lcd_Cmd(LCD_DISPLAY_ON_CURSOR_OFF);

    // 5) Clear display
    Lcd_Cmd(LCD_CLEAR);

    // 6) Delay to allow clear display to finish
    DelayMs(20);
}

extern void Lcd_Cmd(uint8_t cmd){
    uint8_t value = 0;
    switch (cmd){
            case LCD_CLEAR:
                value = 0x01;
                break;
            case LCD_CURSOR_OFF:
                value = 0b00001100;
                break;
            case LCD_UNDERLINE_ON:
                value = 0b00001110;
                break;
            case LCD_BLINK_CURSOR_ON:
                value = 0b00001101;
                break;
            case LCD_FIRST_ROW:
                value = 0b10000000;
                break;
            case LCD_SET_CURSOR_BEGINNING:
                value = 0x80;
                break;
            case LCD_INCREMENT_CURSOR:
                value = 0x80;
                break;
            case LCD_DISPLAY_ON_CURSOR_OFF:
                value = 0x0C;
                break;
            case LCD_FONT_8BIT_CONFIG:
                value = 0x38;
                break;
            default:   // LCD_SECOND_ROW
                value = 0b10000000 + 0x40;
                break;
        }

    Lcd_Send(value);                 // Send command
    GPIO_PORTD_DATA_R &= ~(1<<1);   // R/W = 0 (write)
    GPIO_PORTD_DATA_R &= ~(1<<0);   // RS = 0 (command register)
    GPIO_PORTD_DATA_R |= (1<<2);    // EN = 1 (enable)
    DelayMs(50);          // Delay for command latch
    GPIO_PORTD_DATA_R &= ~(1<<2);   // EN = 0 (disable)
}

extern void lcd_string(unsigned char *str, unsigned char len)
{
    unsigned char i;  // Declare i before using it
    for(i = 0; i < len; i++)
    {
        lcd_data(str[i]);
    }
}

static void lcd_data(unsigned char data)
{
    Lcd_Send(data);                 // Send 8-bit data to the LCD
    GPIO_PORTD_DATA_R &= ~(1<<1);    // R/W = 0 (write)
    GPIO_PORTD_DATA_R |= (1<<0);     // RS = 1 (data register)
    GPIO_PORTD_DATA_R |= (1<<2);     // EN = 1 (enable)
    DelayMs(50);           // Delay for data latch
    GPIO_PORTD_DATA_R &= ~(1<<2);    // EN = 0 (disable)
}


