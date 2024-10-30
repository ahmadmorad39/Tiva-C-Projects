/*
 * halGPIO.c
 *
 *  Created on: Oct 26, 2024
 *      Author: ahmad
 */


#include  "halGPIO.h"     // private library - HAL layer
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/uart.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
//--------------------------------------------------------------------
//             System Configuration
//--------------------------------------------------------------------
void sysConfig(void){
    GPIOconfig();
}

//******************************************************************
// Delay usec functions
//******************************************************************
void DelayUs(unsigned int cnt){
    unsigned char i;
        for(i=cnt ; i>0 ; i--){
            asm(" nop"); // tha command asm("nop") takes raphly 1usec
        }

}
//******************************************************************
// Delay msec functions
//******************************************************************
void DelayMs(uint32_t cnt){
    SysCtlDelay(SysCtlClockGet() * ((float)cnt / 3000.0));
}


//---------------------------------------------------------------------
//            Enter from LPM0 mode
//---------------------------------------------------------------------
// Function to enter low-power mode based on specified level
void enterLPM(uint8_t LPM_level) {
    switch (LPM_level) {
        case 0:
            // Equivalent to a standard sleep mode (similar to LPM0)
            SysCtlSleep();
            break;
        case 1:
            // Equivalent to deep sleep mode (lower power than LPM0)
            SysCtlDeepSleep();
            break;
        case 2:
            // For further low-power modes, additional control over clocks/peripherals would be necessary
            // Custom settings for very low-power modes, e.g., disabling specific peripherals, can be added here
            SysCtlDeepSleep(); // Placeholder, as Tiva has limited discrete LPMs
            break;
        default:
            SysCtlDeepSleep();
            break;
    }
}


//---------------------------------------------------------------------
//            Enable interrupts
//---------------------------------------------------------------------
void enable_interrupts() {
    IntMasterEnable();
}
//---------------------------------------------------------------------
//            Disable interrupts
//---------------------------------------------------------------------
void disable_interrupts() {
    IntMasterDisable();
}

//*********************************************************************
//            Define the ISR for ADC0 Sample Sequencer 0
//*********************************************************************
void ADC0SS0_Handler(void) {
    // Read ADC values
    ADCSequenceDataGet(ADC0_BASE, 0, ADC_vals);
    // Clear the interrupt
    ADC0_IM_R &= ~(1 << 0);
}

//*********************************************************************
//            UART0 Interrupt Handler
//*********************************************************************
void UART0IntHandler(void) {
    // Clear the UART interrupt flag
    UARTIntClear(UART0_BASE, UART_INT_RX);

    // Read character from the UART0 data register
    string[rx_idx] = UARTCharGetNonBlocking(UART0_BASE);
    rx_idx++;

    if (string[rx_idx - 1] == '\n') {
        string[rx_idx - 1] = 0;
        rx_idx = 0;

        // Handling different commands based on received data
        if (string[0] == 'F') {
            switch (string[1]) {
                case '1':
                    copy_2seg(Flash_ptrB, string + 2);
                    break;
                case '2':
                    copy_2seg(Flash_ptrC, string + 2);
                    break;
                case '3':
                    copy_2seg(Flash_ptrD, string + 2);
                    break;
                default:
                    break;
            }
            state = state0;
        } else if (string[0] == 'i') {
            switch (string[1]) {
                case '1':
                    cur_Flash = Flash_ptrB;
                    break;
                case '2':
                    cur_Flash = Flash_ptrC;
                    break;
                case '3':
                    cur_Flash = Flash_ptrD;
                    break;
                default:
                    break;
            }
            state = state3;
        } else if (string[0] == 's') {
            switch (string[1]) {
                case '0':
                    state = state0;
                    break;
                case '1':
                    start_ADC();
                    start_TIMERA0_0();
                    state = state1;
                    break;
                case '2':
                    start_ADC();
                    start_TIMERA0_0();
                    state = state2;
                    break;
                case '6':
                    state = state6;
                    clockwise = 0;
                    motor_o = 8;
                    wake_from_a0 = 1;
                    cal_angle_test = 0;
                    break;
                default:
                    break;
            }
        } else if (string[0] == 'a') {
            // Convert received string to angle and adjust motor direction
            r_angle = char_to_int(string + 1);
            check_motor_dir(r_angle, c_angle, &clockwise, &motor_o);

            // Start timer if required
            if (!(TimerIntStatus(TIMER0_BASE, false) & TIMER_TIMA_TIMEOUT) && (r_angle != c_angle)) {
                TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Enable Timer A0 interrupt
            }
        }
    }
}

//*********************************************************************
//            Timer A0 interrupt service routine
//*********************************************************************
void Timer0A_Handler(void) {
    // Clear the Timer interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    uint32_t pulseWidth = 1000 + (0 * (2000 - 1000) / 180);
    uint32_t compareValue = (pulseWidth * (SysCtlClockGet() / 50)) / 1000000; // Convert to clock cycles
    PWM1_3_CMPA_R = compareValue; // Set the new compare value

    // Motor control logic
    GPIO_PORTB_DATA_R = motor_o;

    if (clockwise) {
        if (motor_o == 8) {
            motor_o = 1;
            c_angle = (c_angle == 0) ? calib_angle : c_angle - 1;
            if (dyn_send) {
                send_current_step(c_angle, tmp_str);
            }
        } else {
            motor_o <<= 1;  // Shift left
        }
    } else {
        if (motor_o == 1) {
            motor_o = 8;
            c_angle = (c_angle == calib_angle) ? 0 : c_angle + 1;
            if (dyn_send) {
                send_current_step(c_angle, tmp_str);
            }
        } else {
            motor_o >>= 1;  // Shift right
        }
    }

    // Stop rotating when desired angle is reached
    if (c_angle == r_angle) {
        TimerIntDisable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);  // Stop timer
        GPIO_PORTB_DATA_R = 0;       // Turn off motor

        if (wake_from_a0) {
            // Handle low-power exit or state change
        }
    }
}



//*********************************************************************
//            Port1 Interrupt Service Rotine
//*********************************************************************
// GPIO Port F ISR for Pin 0
void PortF_INT_Handler(void) {
    // Debounce delay
    DelayMs(10);

    // Check if the interrupt is from Pin 0
    if (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0)) {
        if (state == 2) {
            // Toggle brush if in state 2
            toggle_brush = true;

            // Optional: Send message (implement send_chars as needed)
            // send_chars("TB\n");
        }

        // Clear the interrupt flag for Pin 0
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0);
    }
}

//*********************************************************************
//                    Timer A1 ISR
//*********************************************************************
void TimerA1_Handler(void) {
    // Clear the timer interrupt flag
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    // If the len of sleep is less than a second
    sec_count -= 1;

    if (sec_len == 0) {
        // Exit low-power mode (if applicable)
        // LPM0_EXIT;  // Implement according to your low-power mode handling
    }
    // We counted the second, set the timer to count the milliseconds
    else if (sec_count == 0 && msec_count != 0) {
        // Check if we have milliseconds to count
        if (msec_len == 0) {
            // Exit low-power mode
            // LPM0_EXIT;  // Implement according to your low-power mode handling
            sec_count = sec_len;
        } else {
            // Set the timer to count milliseconds
            TimerLoadSet(TIMER1_BASE, TIMER_A, msec_len * (SysCtlClockGet() / 1000)); // Convert to clock ticks
            msec_count = 0;
        }
    }
    // We counted the milliseconds, wake up and reset the timer to count the seconds
    else if (msec_count == 0) {
        TimerLoadSet(TIMER1_BASE, TIMER_A, 64000);  // Adjust for the desired timing
        msec_count = 1;
        // Exit low-power mode
        // LPM0_EXIT;  // Implement according to your low-power mode handling
        sec_count = sec_len;
    }
}




