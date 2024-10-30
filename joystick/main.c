

/**
 * main.c
 */
#include  "api.h"         // private library - API layer
#include  "app.h"         // private library - APP layer
#include "tm4c123gh6pm_startup_ccs.h"
#include "utils/uartstdio.h"

enum FSMstate state;
enum SYSmode lpm_mode;


unsigned char c_angle_str[10];

uint32_t ADC_vals[4];
unsigned char X_cord[10];
unsigned char Y_cord[10];
unsigned char tmp_str[10];

unsigned char string[132];
unsigned int rx_idx = 0;

unsigned int c_angle = 0;
unsigned int r_angle = 0;
unsigned char motor_o = 0;
unsigned char clockwise = 0;
unsigned char dyn_send = 0;

signed int calib_angle = 510;
unsigned int cal_angle_test = 0;

unsigned char tooge_b = 0;
unsigned char wake_from_a0 = 0;

//010FFh to 01000h is flashh address 64 byte each
unsigned char *Flash_ptrB = (unsigned char *) 0x1080;             // Initialize Flash segment C pointer
unsigned char *Flash_ptrC = (unsigned char *) 0x1040;             // Initialize Flash segment C pointer
unsigned char *Flash_ptrD = (unsigned char *) 0x1000;             // Initialize Flash segment D pointer

unsigned char *cur_Flash;             // Initialize Flash segment D pointer
unsigned char cur_byte;

unsigned int i;
unsigned char j;

unsigned int tmp_num = 0;
unsigned int tmp_num1 = 0;
unsigned int cur_LED = 0;

unsigned int sec_count = 0;
unsigned int msec_count= 1;

unsigned int sec_len = 0;
unsigned int msec_len = 32500;

unsigned long int long_tmp = 0;
unsigned char toggle_brush = 0;

void main(void) {
    sysConfig(); // Configure system settings
    state = state0; // Start in idle state on RESET
    lpm_mode = mode0; // Start in idle mode

    while (1) {
        switch (state) {
            case state0:
                // Enter low power mode
                //set_rgb(0);
                //set_led(0);
                //stop_ADC();
                stop_Timers();
                SysCtlSleep(); // Enter LPM0 with interrupts enabled
                break;

            case state1:
                sample_convert(); // Convert ADC samples
                tochar(ADC_vals[1], X_cord);
                tochar(ADC_vals[0], Y_cord);
                send_joystick_cords(X_cord, Y_cord);
                SysCtlSleep(); // Enter LPM0 with interrupts enabled
                break;

            case state2:
                sample_convert();
                tochar(ADC_vals[0], X_cord);
                tochar(ADC_vals[1], Y_cord);
                if (toggle_brush) {
                    toggle_brush = false;
                    send_chars("TB"); // Toggle the drawboard brush
                    send_chars("\n");
                } else {
                    send_joystick_cords(X_cord, Y_cord);
                }
                SysCtlSleep(); // Enter LPM0 with interrupts enabled
                break;

            case state3: {
                uint8_t cur_byte = *cur_Flash++; // Read current byte from flash
                switch (cur_byte) {
                    case 1: {
                        // Blink RGB
                        cur_byte = *cur_Flash++; // Read next byte
                        start_TIMERA1();
                        uint8_t j;
                        for (j = 0; j < cur_byte; j++) {
                            set_rgb(0xE0); // Set RGB to a color
                            SysCtlSleep(); // Enter LPM0
                            set_rgb(0); // Turn off RGB
                            SysCtlSleep(); // Enter LPM0
                        }
                        stoptimer_A1_0(); // Stop timer
                        break;
                    }
                    case 2: {
                        // Rotate LEDs
                        cur_byte = *cur_Flash++; // Read next byte
                        start_TIMERA1();
                        if (cur_LED == 0) {
                            cur_LED = 0x10; // Initialize LED
                            set_led(cur_LED);
                        }
                        uint8_t j;
                        for (j = 0; j < cur_byte; j++) {
                            cur_LED = (cur_LED == 0x80) ? 0x10 : (cur_LED << 1);
                            set_led(cur_LED);
                            SysCtlSleep(); // Enter LPM0
                        }
                        stoptimer_A1_0(); // Stop timer
                        break;
                    }
                    case 3: {
                        // Reverse rotate LEDs
                        cur_byte = *cur_Flash++; // Read next byte
                        start_TIMERA1();
                        if (cur_LED == 0) {
                            cur_LED = 0x80; // Initialize LED
                            set_led(cur_LED);
                            SysCtlSleep(); // Enter LPM0
                        }
                        uint8_t j;
                        for (j = 0; j < cur_byte + 1; j++) {
                            cur_LED = (cur_LED == 0x10) ? 0x80 : (cur_LED >> 1);
                            set_led(cur_LED);
                            SysCtlSleep(); // Enter LPM0
                        }
                        stoptimer_A1_0(); // Stop timer
                        break;
                    }
                    case 4: {
                        // Set delay
                        cur_byte = *cur_Flash++; // Read next byte
                        // Implement delay handling logic here
                        break;
                    }
                    case 5: {
                        // Clear all LEDs
                        cur_LED = 0;
                        set_led(cur_LED);
                        break;
                    }
                    case 6: {
                        // Stepper motor control
                        dyn_send = 1;
                        cur_byte = *cur_Flash++;
                        change_cur_angle(&r_angle, calib_angle, cur_byte);
                        check_motor_dir(r_angle, c_angle, &clockwise, &motor_o); // Change direction
                        wake_from_a0 = 1;
                        start_TIMERA0_0();
                        start_stepper();
                        SysCtlSleep(); // Enter LPM0
                        wake_from_a0 = 0;
                        stop_Timers();
                        dyn_send = 0;
                        break;
                    }
                    case 7: {
                        // Additional stepper motor control
                        cur_byte = *cur_Flash++;
                        change_cur_angle(&r_angle, calib_angle, cur_byte);
                        check_motor_dir(r_angle, c_angle, &clockwise, &motor_o); // Change direction
                        wake_from_a0 = 1;
                        start_TIMERA0_0();
                        start_stepper();
                        SysCtlSleep(); // Enter LPM0
                        send_current_step(c_angle, tmp_str);
                        cur_byte = *cur_Flash++;
                        change_cur_angle(&r_angle, calib_angle, cur_byte);
                        check_motor_dir(r_angle, c_angle, &clockwise, &motor_o); // Change direction
                        start_stepper();
                        SysCtlSleep(); // Enter LPM0
                        send_current_step(c_angle, tmp_str);
                        break;
                    }
                    case 8: {
                        // Sleep state
                        state = state0;
                        break;
                    }
                    default:
                        state = state0; // Reset to idle state for any unknown command
                        break;
                }
                break;
            }

            case state5:
                // Handle state5 logic
                break;

            case state6:
                cal_angle_test += 1;
                r_angle = c_angle + 1;
                r_angle = (r_angle == calib_angle) ? 0 : r_angle; // Reset if equal to calibration angle
                start_TIMERA0_0();
                start_stepper();
                SysCtlSleep(); // Enter LPM0
                send_current_cal(cal_angle_test, tmp_str);
                break;

            case state7:
                // Handle state7 logic
                break;

            case state8:
                state = state0; // Reset to idle state
                break;

            default:
                state = state0; // Reset to idle state for any unknown case
                break;
        }
    }
}
