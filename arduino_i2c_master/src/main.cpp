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

void writeByte(char deviceAddress, char bufferAddress, char data) {
  Wire.beginTransmission(deviceAddress);
  Wire.write(bufferAddress);
  Wire.write(data);
  Wire.endTransmission();
}

void read3bytesState() {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(R_ADDRESS);
  Wire.endTransmission();
  Wire.requestFrom(I2C_SLAVE_ADDRESS, 3);
  delay(10);
  Serial.print("read: ");
  while (Wire.available()) {
    Serial.print(Wire.read(), HEX);
    Serial.print(" ");
  }
  Serial.println("");
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

void loop() {
  Serial.println("turn R on");
  writeByte(I2C_SLAVE_ADDRESS, R_ADDRESS, 1);
  read3bytesState();
  read1byteState();
  delay(1000);

  Serial.println("turn G on");
  writeByte(I2C_SLAVE_ADDRESS, G_ADDRESS, 1);
  read3bytesState();
  read1byteState();
  delay(1000);

  Serial.println("turn B on");
  writeByte(I2C_SLAVE_ADDRESS, B_ADDRESS, 1);
  read3bytesState();
  read1byteState();
  delay(1000);

  Serial.println("turn R off");
  writeByte(I2C_SLAVE_ADDRESS, R_ADDRESS, 0);
  read3bytesState();
  read1byteState();
  delay(1000);

  Serial.println("R:on,  G:on,  B:on");
  writeByte(I2C_SLAVE_ADDRESS, RGB_ADDRESS, 0b111);
  read3bytesState();
  read1byteState();
  delay(1000);

  Serial.println("R:off, G:on,  B:on");
  writeByte(I2C_SLAVE_ADDRESS, RGB_ADDRESS, 0b011);
  read3bytesState();
  read1byteState();
  delay(1000);

  Serial.println("R:off, G:off, B:on");
  writeByte(I2C_SLAVE_ADDRESS, RGB_ADDRESS, 0b001);
  read3bytesState();
  read1byteState();
  delay(1000);

  Serial.println("R:off, G:off, B:off");
  writeByte(I2C_SLAVE_ADDRESS, RGB_ADDRESS, 0);
  read3bytesState();
  read1byteState();
  delay(1000);
}
