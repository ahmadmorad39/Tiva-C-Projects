#include "inc/tm4c123gh6pm.h"
#include <stdint.h>

void delayMicroseconds(long d);
void Printdata(unsigned char data);
void lcd_data(unsigned char data);
void lcd_cmd(unsigned char cmd);
void lcd_string(unsigned char *str, unsigned char len);
void lcd_init(void);

// RS = PD0
// RW = PD1
// EN = PD2

// D0 = PA7
// D1 = PA6
// D2 = PA5
// D3 = PB4
// D4 = PE5
// D5 = PE4
// D6 = PB1
// D7 = PB0

int main(void)
{
    // Enable clocks for PORTA, PORTB, PORTD, PORTE
    SYSCTL_RCGCGPIO_R |= (1<<0) | (1<<1) | (1<<3) | (1<<4);
    delayMicroseconds(10000);  // Wait for the clock to stabilize

    // Unlock and commit for PORTB (if needed)
    GPIO_PORTB_LOCK_R = 0x4C4F434B;
    GPIO_PORTB_CR_R |= (1<<0) | (1<<1) | (1<<4);

    // Digitalize data pins and control pins
    GPIO_PORTA_DEN_R |= (1<<5) | (1<<6) | (1<<7);
    GPIO_PORTB_DEN_R |= (1<<0) | (1<<1) | (1<<4);
    GPIO_PORTD_DEN_R |= (1<<0) | (1<<1) | (1<<2);
    GPIO_PORTE_DEN_R |= (1<<4) | (1<<5);

    // Set direction for data pins and control pins (outputs)
    GPIO_PORTA_DIR_R |= (1<<5) | (1<<6) | (1<<7);
    GPIO_PORTB_DIR_R |= (1<<0) | (1<<1) | (1<<4);
    GPIO_PORTD_DIR_R |= (1<<0) | (1<<1) | (1<<2);
    GPIO_PORTE_DIR_R |= (1<<4) | (1<<5);

    delayMicroseconds(10000);  // Additional delay for stabilization

    lcd_init();           // Initialize the LCD
    lcd_cmd(0x80);        // Set cursor to beginning of first line
    lcd_string("Hello World", 11);        // Display the letter 'a'
    while(1) {
            // Main loop: You can add more functionality here
        }
    return 0;
}

void delayMicroseconds(long d)
{
    volatile long i;
    while(d--) {
        i = 500;  // Tweak this based on your system clock speed
        while(i--);
    }
}

void Printdata(unsigned char data)
{
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

void lcd_data(unsigned char data)
{
    Printdata(data);                 // Send 8-bit data to the LCD
    GPIO_PORTD_DATA_R &= ~(1<<1);    // R/W = 0 (write)
    GPIO_PORTD_DATA_R |= (1<<0);     // RS = 1 (data register)
    GPIO_PORTD_DATA_R |= (1<<2);     // EN = 1 (enable)
    delayMicroseconds(50);           // Delay for data latch
    GPIO_PORTD_DATA_R &= ~(1<<2);    // EN = 0 (disable)
}

void lcd_cmd(unsigned char cmd)
{
    Printdata(cmd);                 // Send command
    GPIO_PORTD_DATA_R &= ~(1<<1);   // R/W = 0 (write)
    GPIO_PORTD_DATA_R &= ~(1<<0);   // RS = 0 (command register)
    GPIO_PORTD_DATA_R |= (1<<2);    // EN = 1 (enable)
    delayMicroseconds(50);          // Delay for command latch
    GPIO_PORTD_DATA_R &= ~(1<<2);   // EN = 0 (disable)
}

void lcd_string(unsigned char *str, unsigned char len)
{
    unsigned char i;  // Declare i before using it
    for(i = 0; i < len; i++)
    {
        lcd_data(str[i]);
    }
}
void lcd_init(void)
{
    lcd_cmd(0x38);  // 8-bit mode, 2 lines, 5x7 matrix
    delayMicroseconds(50000);
    lcd_cmd(0x06);  // Increment cursor
    lcd_cmd(0x0C);  // Display ON, cursor Off
    lcd_cmd(0x01);  // Clear display
    delayMicroseconds(2000);  // Delay to allow clear display to finish
}
