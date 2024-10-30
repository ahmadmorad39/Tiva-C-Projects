/*
 * halGPIO.h
 *
 *  Created on: Oct 26, 2024
 *      Author: ahmad
 */

#ifndef HALGPIO_H_
#define HALGPIO_H_

// ================================= Header files ====================================
#include  "bsp.h"         // private library - BSP layer
#include  "app.h"         // private library - APP layer

// ================================= variable =========================================
extern enum FSMstate state;   // global variable
extern enum SYSmode lpm_mode; // global variable

////////////////////////////////
/////GLOBAL VARIABLES
///////////////////////////////

extern uint32_t ADC_vals[4];
extern unsigned char X_cord[10];
extern unsigned char Y_cord[10];
extern unsigned char tmp_str[10];
extern unsigned char c_angle_str[10];

extern unsigned char string[132];
extern unsigned int rx_idx;

extern unsigned int c_angle;
extern unsigned int r_angle;
extern unsigned char motor_o;
extern unsigned char clockwise;
extern unsigned char dyn_send;
extern signed int calib_angle;
extern unsigned int cal_angle_test;

extern unsigned char tooge_b;
extern unsigned char wake_from_a0;


extern unsigned char * Flash_ptrB;             // Initialize Flash segment C pointer
extern unsigned char * Flash_ptrC;             // Initialize Flash segment C pointer
extern unsigned char *Flash_ptrD;             // Initialize Flash segment D pointer
extern unsigned char *cur_Flash ;

extern unsigned int sec_count;
extern unsigned int msec_count;
extern unsigned int sec_len;
extern unsigned int msec_len;

extern unsigned char toggle_brush;
///////////////////////////////

// ================================= Function prototypes ===========================================
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern void sysConfig(void);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern void lcd_data(unsigned char);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern void disable_interrupts();
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern void enable_interrupts();
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern void enterLPM(unsigned char);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern void DelayMs(unsigned int);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern void DelayUs(unsigned int);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern void send_chars(unsigned char* str);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern void copy_2seg(unsigned char *,unsigned char* );
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern unsigned int char_to_int(unsigned char *);
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern void send_current_step(unsigned int,unsigned char* );
/**
 * *************************************************************************************************
 * @brief
 * @param
 * @return
 * *************************************************************************************************
 */
extern void check_motor_dir(unsigned int ,unsigned int ,unsigned char *,unsigned char *);


#endif /* HALGPIO_H_ */
