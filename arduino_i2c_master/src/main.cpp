#include <Arduino.h>
#include <Wire.h>

#define I2C_SLAVE_ADDRESS 0x10
#define RGB_ADDRESS 0x00
#define R_ADDRESS   0x01
#define G_ADDRESS   0x02
#define B_ADDRESS   0x03

void setup() {
  Wire.begin();
  Serial.begin(115200);
  delay(500);
}

uint8_t writeByte(char deviceAddress, char bufferAddress, char data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(bufferAddress);
  Wire.write(data);
  return Wire.endTransmission();
}

uint8_t read3bytesState() {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(R_ADDRESS);
  uint8_t result = Wire.endTransmission();
  if (result != 0) return result;
  Wire.requestFrom(I2C_SLAVE_ADDRESS, 3);
  delay(10);
  Serial.print("read:");
  while (Wire.available()) {
    Serial.print(" ");
    Serial.print(Wire.read(), HEX);
  }
  Serial.println("");
  return 0;
}

void read1byteState() {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(RGB_ADDRESS);
  Wire.endTransmission();
  Wire.requestFrom(I2C_SLAVE_ADDRESS, 1);
  delay(10);
  Serial.print("read: ");
  while (Wire.available()) {
    Serial.print(Wire.read(), BIN);
  }
  Serial.println("");
}

void writeAndReadResults(char deviceAddress, char bufferAddress, char data) {
  if (writeByte(deviceAddress, bufferAddress, data) != 0) {
    Serial.println("Failed to write");
  } else {
    read3bytesState();
    read1byteState();
  }
}

void loop() {
  Serial.println("turn R on");
  writeAndReadResults(I2C_SLAVE_ADDRESS, R_ADDRESS, 1);
  delay(1000);

  Serial.println("turn G on");
  writeAndReadResults(I2C_SLAVE_ADDRESS, G_ADDRESS, 1);
  delay(1000);

  Serial.println("turn B on");
  writeAndReadResults(I2C_SLAVE_ADDRESS, B_ADDRESS, 1);
  delay(1000);

  Serial.println("turn R off");
  writeAndReadResults(I2C_SLAVE_ADDRESS, R_ADDRESS, 0);
  delay(1000);

  Serial.println("turn G off");
  writeAndReadResults(I2C_SLAVE_ADDRESS, G_ADDRESS, 0);
  delay(1000);

  Serial.println("turn B off");
  writeAndReadResults(I2C_SLAVE_ADDRESS, B_ADDRESS, 0);
  delay(1000);

  Serial.println("R:off,  G:off,  B:on");
  writeAndReadResults(I2C_SLAVE_ADDRESS, RGB_ADDRESS, 0b001);
  delay(1000);

  Serial.println("R:off, G:on,  B:on");
  writeAndReadResults(I2C_SLAVE_ADDRESS, RGB_ADDRESS, 0b011);
  delay(1000);

  Serial.println("R:on,  G:on,  B:on");
  writeAndReadResults(I2C_SLAVE_ADDRESS, RGB_ADDRESS, 0b111);
  delay(1000);

  Serial.println("R:on, G:on,  B:off");
  writeAndReadResults(I2C_SLAVE_ADDRESS, RGB_ADDRESS, 0b110);
  delay(1000);

  Serial.println("R:on, G:off, B:off");
  writeAndReadResults(I2C_SLAVE_ADDRESS, RGB_ADDRESS, 0b100);
  delay(1000);

  Serial.println("R:off, G:off, B:off");
  writeAndReadResults(I2C_SLAVE_ADDRESS, RGB_ADDRESS, 0);
  delay(1000);
}
