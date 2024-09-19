/*
 * lcd.h
 *
 *  Created on: Sep 18, 2024
 *      Author: ahmad
 */

#ifndef LCD_H_
#define LCD_H_

// ================================= Header files ====================================
// Defines integer data and boolean types
#include <stdint.h>
#include <stdbool.h>

// ================================= Macro ===========================================
// LCD connection pins
// -------------------
// Data port
// -------------------
#define LCD_DATA_PORT1                      GPIO_PORTA_BASE

#define LCD_D0                              GPIO_PIN_7
#define LCD_D1                              GPIO_PIN_6
#define LCD_D2                              GPIO_PIN_5

#define LCD_DATA_PORT2                      GPIO_PORTB_BASE
#define LCD_D3                              GPIO_PIN_4
#define LCD_D6                              GPIO_PIN_1
#define LCD_D7                              GPIO_PIN_0

#define LCD_DATA_PORT3                      GPIO_PORTE_BASE
#define LCD_D4                              GPIO_PIN_5
#define LCD_D5                              GPIO_PIN_4

#define LCD_DATA_PORT_ENABLE1               SYSCTL_PERIPH_GPIOA
#define LCD_DATA_PORT_ENABLE2               SYSCTL_PERIPH_GPIOB
#define LCD_DATA_PORT_ENABLE3               SYSCTL_PERIPH_GPIOE
// Control port
// ------------
#define LCD_CONTROL_PORT                    GPIO_PORTD_BASE

#define LCD_RS                              GPIO_PIN_0
#define LCD_RW                              GPIO_PIN_1
#define LCD_EN                              GPIO_PIN_2

#define LCD_CONTROL_PORT_ENABLE             SYSCTL_PERIPH_GPIOD

// Available LCD Commands
// ----------------------
#define LCD_CLEAR                           1
#define LCD_CURSOR_OFF                      2
#define LCD_UNDERLINE_ON                    3
#define LCD_BLINK_CURSOR_ON                 4
#define LCD_FIRST_ROW                       5
#define LCD_SECOND_ROW                      6
#define LCD_SET_CURSOR_BEGINNING            7
#define LCD_INCREMENT_CURSOR                8
#define LCD_DISPLAY_ON_CURSOR_OFF           9
#define LCD_FONT_8BIT_CONFIG                10
// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void Lcd_Init(void);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void Lcd_Cmd(uint8_t cmd);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
void lcd_string(unsigned char *str, unsigned char len);

#endif /* LCD_H_ */
