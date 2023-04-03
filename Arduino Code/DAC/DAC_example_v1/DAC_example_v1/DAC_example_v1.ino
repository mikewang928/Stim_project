#include <SPI.h>

// we will first start from a unipolar design (0-10 volt)

// DAC control pins
const int CS_PIN = 10; // chip select pin
const int LDAC_PIN = 9; // load DAC pin

// DAC command codes
const byte DAC_WRITE = 0x00; // write to DAC register
// const byte DAC_LOAD = 0x80; // load DAC output register

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
  pinMode(CS_PIN, OUTPUT); // chip select pin
  pinMode(LDAC_PIN, OUTPUT); // if load DAC pin
  digitalWrite(CS_PIN, HIGH); // disable chip select by default
  digitalWrite(LDAC_PIN, HIGH); // disable load DAC by default ( DAC A and DAC B are updated with information in the input register on the LDAC falling edge)
}

// --------------------------------------------- helper functions --------------------------------------------------
// convert voltage to DAC value
int voltsToDAC(float volts) {
  volts = constrain(volts, DAC_MIN_VOLTS, DAC_MAX_VOLTS);
  int value = map(volts, DAC_MIN_VOLTS, DAC_MAX_VOLTS, DAC_MIN_VALUE, DAC_MAX_VALUE);
  return value;
}

// write DAC output value
void writeDAC(int value) {
  digitalWrite(CS_PIN, LOW); // enable chip select
  SPI.transfer(DAC_WRITE); // send write command
  SPI.transfer((value >> 12) & 0xFF); // send MSB of value
  SPI.transfer(value & 0xFF); // send LSB of value
  digitalWrite(CS_PIN, HIGH); // disable chip select
}

// load DAC output value
void loadDAC() {
  digitalWrite(LDAC_PIN, LOW); // enable load DAC
  digitalWrite(LDAC_PIN, HIGH); // disable load DAC
}

// set DAC output voltage
void setDAC(float volts) {
  int value = voltsToDAC(volts);
  writeDAC(value);
  loadDAC();
}

// main program loop
void loop() {
  // set DAC output voltage to 1.0 V
  setDAC(1.0);
  
  // wait for 1 second
  delay(1000);
  
  // set DAC output voltage to 2.0 V
  setDAC(2.0);
  
  // wait for 1 second
  delay(1000);
}
