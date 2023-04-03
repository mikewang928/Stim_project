#include <SPI.h>

// DAC control pins
const int CS_PIN = 10; // chip select pin
const int LDAC_PIN = 9; // load DAC pin

// DAC output voltage range (in volts)
const float DAC_MIN_VOLTS = 0.0;
const float DAC_MAX_VOLTS = 10;

// DAC output value range (12-bit resolution)
const int DAC_MIN_VALUE = 0;
const int DAC_MAX_VALUE = 4095;

// initialize SPI interface
void setup() {
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2); // set SPI clock speed to 8 MHz (16 MHz / 2)
  SPI.beginTransaction(SPISettings(10000000, LSBFIRST, SPI_MODE0)); // 10MHz and rising edge capture 
  // SPISettings mySetting(10000000, LSBFIRST, dataMode)
  // SPI communication settings 
  pinMode(CS_PIN, OUTPUT);
  pinMode(LDAC_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH); // disable chip select by default
  digitalWrite(LDAC_PIN, HIGH); // disable load DAC by default
}

// convert voltage to DAC value
// return value is a 12-bit DAC number 
int voltsToDAC(float volts) {
  volts = constrain(volts, DAC_MIN_VOLTS, DAC_MAX_VOLTS);
  int value = map(volts, DAC_MIN_VOLTS, DAC_MAX_VOLTS, DAC_MIN_VALUE, DAC_MAX_VALUE);
  return value;
}

// write DAC output value in both to the input register and DAC register
// value 16 bit binary signal (4:command signal (0001); 12 DAC value)
void writeDAC(int value) {
  digitalWrite(CS_PIN, LOW); // enable chip select
  SPI.transfer(value);
  digitalWrite(CS_PIN, HIGH); // disable chip select
}

// set DAC output voltage
void setDAC(float volts) {
  int value = voltsToDAC(volts)+0x1000; // adding the command bits
  writeDAC(value);
}

// main program loop
void loop() {
  // set DAC output voltage to 1.0 V
  setDAC(1.0);
  // wait for 1 second
  delay(1000);
  // set DAC output voltage to 2.0 V
  setDAC(8.0);
  // wait for 1 second
  delay(1000);
}
