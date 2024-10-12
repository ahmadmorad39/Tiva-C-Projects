#include <SPI.h>

volatile byte s_recv;     // Byte to store received data
volatile bool flag = 0;   // Flag to check if data has been received

void setup() {
  Serial.begin(9600);      // Initialize serial monitor at 9600 baud rate
  
  // Set up MISO as output since this is the slave and needs to send data
  pinMode(MISO, OUTPUT);   

  // Enable SPI, set as slave, and enable SPI interrupt
  SPCR |= (1 << SPE) | (1 << SPIE);  // Enable SPI and SPI interrupt
  sei();  // Enable global interrupts
}

void loop() {
  if (flag) {
    Serial.print("Received byte: ");
    Serial.println((char)s_recv);   // Print received byte as a character
    flag = 0;  // Reset flag after processing
  }
  delay(500);  // Delay to make serial output more readable
}

// SPI interrupt handler
ISR(SPI_STC_vect) {
  s_recv = SPDR;  // Read the received data from the SPI Data Register
  flag = 1;       // Set the flag to indicate data is received
}
