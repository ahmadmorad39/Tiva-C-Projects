/*
 * api.c
 *
 *  Created on: Oct 26, 2024
 *      Author: ahmad
 */

#include  "api.h"         // private library - API layer
#include  "halGPIO.h"     // private library - HAL layer
#include "driverlib/sysctl.h" // For SysCtl functions
#define SYSCTL_PERIPH_FLASH0  0x00000001  // Example definition


unsigned char *tochar(unsigned int i,unsigned char *p)
{
    if (i >> 4 == 0) {
        // No more digits.
        *p++ = (i < 10 ? i + '0' : i  + '7');
        *p = '\0';
        return p;
    }
    p = tochar(i>> 4, p);
    *p++ = ( (i & 0xF) < 10 ? (i & 0xF) + '0' : (i & 0xF) +'7');
    *p = '\0';
    return p;
}

// Function to send characters via UART
void send_chars(unsigned char* str) {
    while (*str) {
        while (UARTBusy(UART0_BASE)); // Wait until UART is ready
        UARTCharPut(UART0_BASE, *str++); // Transmit the character
    }
}


void send_joystick_cords(unsigned char *str_x,unsigned char *str_y){
    send_chars("XY_");
    send_chars(str_x);
    send_chars("_");
    send_chars(str_y);
    send_chars("\n");
}

unsigned int char_to_int(unsigned char *str){
    unsigned int num = 0;
    unsigned char tmp_char;
    while(*str != 0){
        num = (num << 4);
        tmp_char = *str++;
        num += (tmp_char < 'A' ? tmp_char - '0' : tmp_char - 'A' +  10);
    }
    return num;
}

void check_motor_dir(unsigned int r_angle,unsigned int c_angle,unsigned char *clockwise,unsigned char *motor_o){
    if(r_angle == c_angle){
        return;
    }
    if(r_angle > c_angle){
        if(r_angle - c_angle < 255){
            //move counter clockwise
            *clockwise = 0;
            *motor_o = 8;
        }else{
            *clockwise = 1;
            *motor_o = 1;
        }
    }else{
        if(c_angle - r_angle < 255){
            *clockwise = 1;
            *motor_o = 1;
        }else{
            *clockwise = 0;
            *motor_o = 8;

        }
    }
}


// Flash memory operations
void copy_2seg(unsigned char *Flash_ptr, unsigned char *str_ptr) {
    // Ensure Flash module is enabled
    // Use the correct peripheral definition based on your microcontroller
    SysCtlPeripheralEnable(SYSCTL_PERIPH_FLASH0); // Check if this exists in your version
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_FLASH0));

    // Erase the flash before writing
    FlashErase((uint32_t)Flash_ptr);

    // Write data to flash
    FlashProgram((void *)str_ptr, (uint32_t)Flash_ptr, strlen((const char *)str_ptr));

    // Additional error checking or finalization can be added here
}




void send_current_step(unsigned int c_angle,unsigned char* tmp_str){
    send_chars("CA_");
    tochar(c_angle,tmp_str);
    send_chars(tmp_str);
    send_chars("\n");
}

void send_current_cal(unsigned int c_angle,unsigned char* tmp_str){
    send_chars("CC_");
    tochar(c_angle,tmp_str);
    send_chars(tmp_str);
    send_chars("\n");
}


void change_cur_angle(unsigned int* cur_angle,unsigned int calib_angle,unsigned char num){
    unsigned long int long_tmp = num;
    long_tmp = long_tmp*calib_angle;
    long_tmp = long_tmp/360;
    *cur_angle = long_tmp;
}


void calc_pause(unsigned char val,unsigned int* sec_len,unsigned int* msec_len,unsigned int* sec_count,unsigned int* msec_count){
    unsigned int tmp_num = val;
    *sec_len = tmp_num/100;
    tmp_num = tmp_num%100;
    *msec_len = (tmp_num + (tmp_num<<6))*10;//msec_len = val*65*10
    *sec_count = *sec_len;
    *msec_count = 1;

}




