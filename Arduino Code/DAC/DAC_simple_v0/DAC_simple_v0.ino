#include <SPI.h>

// define your 16-bit command here
uint16_t command = 0xABCD;

void setup() {
  // initialize SPI communication
  SPI.begin();
  // configure SPI settings
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  // send the 16-bit command
  SPI.transfer16(command);
  // delay for a bit
  delay(100);
}
