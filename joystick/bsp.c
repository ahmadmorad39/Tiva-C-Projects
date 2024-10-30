/*
 * bsp.c
 *
 *  Created on: Oct 26, 2024
 *      Author: ahmad
 */


#include  "bsp.h"    // private library - BSP layer
//-----------------------------------------------------------------------------
//           GPIO congiguration
//-----------------------------------------------------------------------------
void GPIOconfig(void) {
    // Set the clocking to run at 50 MHz from the PLL.
    SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_SYSDIV_2_5);
    // Stepper Motor Configuration PortB (pin0,1,2,3)
    // Enable the PWM module
    SYSCTL_RCGCPWM_R |= (1<<1);   // Enable clock for PWM1
    SYSCTL_RCGCGPIO_R |= (1<<5);   // Enable clock for GPIO port F
    SYSCTL_RCC_R &= (~(1<<20));     // Clear the PWM divider

    // Configure GPIO for PWM function
    GPIO_PORTF_AFSEL_R |= (1<<2);  // Enable alternate function for PF2
    GPIO_PORTF_PCTL_R = 0x00000500; // Set PF2 to PWM output
    GPIO_PORTF_DEN_R |= (1<<2);     // Enable digital functionality for PF2

    // Configure PWM generator 3
    PWM1_3_CTL_R &= (~(1<<0));      // Disable PWM generator 3
    PWM1_3_CTL_R &= (~(1<<1));      // Disable PWM generator 3 for trigger mode
    PWM1_3_LOAD_R = 16000;          // Set the PWM load value (frequency)
    PWM1_3_CMPA_R = 8000;           // Set compare value for 50% duty cycle
    PWM1_3_GENA_R |= (1<<2) | (1<<3) | (1<<7); // Configure PWM output
    PWM1_3_CTL_R |= (1<<0);         // Enable PWM generator 3
    PWM1_ENABLE_R |= (1<<6);         // Enable PWM output for PWM1 generator 3
    // --------------------------------------------------------------------------------------------------
    // Configure P1.0 as input with pull-down
    // Enable the clock to PORTF
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // Wait for PORTF to be ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)) {}

    // Unlock PORTF0 for configuration
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;  // Lock after modification

    // Configure PF0 as input and enable pull-up resistor
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Configure interrupt for PF0 with falling edge trigger
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0);

    // Enable interrupt in NVIC
    IntEnable(INT_GPIOF);
    // ----------------------------------------------------------------------------------------------------
    // Initialize UART0
    // Enable UART0 and GPIOA peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    // Configure GPIO pins for UART mode (PA0 -> U0RX, PA1 -> U0TX)
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // Configure UART clock and baud rate (e.g., 115200 baud)
    UARTStdioConfig(0, 115200, SysCtlClockGet());
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT); // Enable RX and RT interrupts
    IntEnable(INT_UART0); // Enable the UART0 interrupt in the NVIC
    // ----------------------------------------------------------------------------------------------------
    // Enable ADC0 and GPIO E peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);

    // Wait for peripherals to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_ADC0)) {}
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)) {}

    // Configure PE3, PE2, and PE1 for analog input
    GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1);

    // Configure the ADC sequencer
    ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);

    // Configure steps in the sequence
    ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0); // First sample on AIN0 (PE3)
    ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1); // Second sample on AIN1 (PE2)
    ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2 | ADC_CTL_IE | ADC_CTL_END); // Third sample on AIN2 (PE1) with interrupt and end of sequence

    // Enable the ADC sequence
    ADCSequenceEnable(ADC0_BASE, 0);

    // Clear any existing ADC interrupt
    ADCIntClear(ADC0_BASE, 0);

    // Enable the ADC interrupt in the NVIC
    IntEnable(INT_ADC0SS0); // Enable the interrupt for ADC0 sequencer 0

    // Enable global interrupts
    IntMasterEnable(); // Enable global interrupts
    // ----------------------------------------------------------------------------------------------------
    // Timer configuration
    // Timer0 configuration (periodic)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)); // Ensure Timer0 is ready
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, 200); // Set the load value for Timer0
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER0_BASE, TIMER_A); // Start Timer0A
    // ----------------------------------------------------------------------------------------------------
    // Timer1 configuration (periodic)
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)); // Ensure Timer1 is ready
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER1_BASE, TIMER_A, 64000); // Set default load value for Timer1
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE, TIMER_A); // Start Timer1A
}

//-------------------------------------------------------------------------------------
//            Timers congiguration
//-------------------------------------------------------------------------------------

void start_TIMERA0_0(void) {
    // Set the timer load value to 200
    TimerLoadSet(TIMER0_BASE, TIMER_A, 200);

    // Disable Timer0A interrupt
    TimerIntDisable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Start Timer0A
    TimerEnable(TIMER0_BASE, TIMER_A);
}

// Function to stop Timer A1
void stoptimer_A1_0(void) {
    // Disable Timer A1
    TimerDisable(TIMER1_BASE, TIMER_A); // Disable Timer A of Timer 1

    // Clear any pending interrupt flags for Timer A1
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}

void change_TA0CCR0(uint32_t val) {
    TimerLoadSet(TIMER0_BASE, TIMER_A, val);
}

void start_TIMERA1(uint32_t sec_len, uint32_t msec_len) {
    // Enable the peripheral clock for Timer1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)); // Ensure Timer1 is ready

    // Configure Timer1 as periodic
    TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);

    // Set load value for Timer1 depending on sec_len and msec_len values
    TimerLoadSet(TIMER1_BASE, TIMER_A, (sec_len == 0) ? msec_len : 64000);

    // Enable Timer1 timeout interrupt
    TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

    // Start Timer1A
    TimerEnable(TIMER1_BASE, TIMER_A);
}

void stop_Timers(void) {
    // Disable Timer0A and Timer1A
    TimerDisable(TIMER0_BASE, TIMER_A);
    TimerDisable(TIMER1_BASE, TIMER_A);

    // Disable timeout interrupts for Timer0 and Timer1
    TimerIntDisable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    TimerIntDisable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}

void set_rgb(uint8_t val) {
    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2, (0xE0 & val));
}

void set_led(uint8_t val) {
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, (0xF0 & val));
}

void start_stepper(void) {
    // Clear any pending Timer0A interrupt
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Enable the Timer0A timeout interrupt
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

    // Enable Timer0A interrupt in the NVIC
    IntEnable(INT_TIMER0A);

    // Start Timer0A
    TimerEnable(TIMER0_BASE, TIMER_A);
}

//-------------------------------------------------------------------------------------
//            ADC congiguration
//-------------------------------------------------------------------------------------
void start_ADC(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0); // Use sequence 3
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH4 | ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END); // Read channels
    ADCSequenceEnable(ADC0_BASE, 3);
    ADCIntClear(ADC0_BASE, 3);
}

void stop_ADC(void) {
    // Disable ADC0 Sample Sequencer 3
    ADC0_ACTSS_R &= ~0x08;  // Disable sample sequencer 3

    // Set the input channel to AIN3, which is PE0 on Tiva C
    SYSCTL_RCGCGPIO_R |= 0x10;            // Enable clock to Port E
    while ((SYSCTL_PRGPIO_R & 0x10) == 0); // Wait until Port E is ready
    GPIO_PORTE_AFSEL_R |= 0x01;           // Enable alternate function on PE0
    GPIO_PORTE_DEN_R &= ~0x01;            // Disable digital I/O on PE0
    GPIO_PORTE_AMSEL_R |= 0x01;           // Enable analog function on PE0

    // Configure ADC0 Sample Sequencer 3 to use AIN3
    ADC0_SSMUX3_R = 3;                    // Set channel AIN3 (PE0)

    // Disable the ADC by disabling sample sequencers and clearing configurations
    ADC0_ACTSS_R &= ~0x0F;                // Disable all sample sequencers
}

// Function to start ADC conversions
void sample_convert(void) {
    IntMasterEnable();
    IntEnable(INT_ADC0SS0);
    ADC0_IM_R |= (1 << 0); // Enable interrupts for sequence 0
    // Trigger the ADC conversion
    ADCProcessorTrigger(ADC0_BASE, 0);
    while (!ADCIntStatus(ADC0_BASE, 0, false)) {}
}
