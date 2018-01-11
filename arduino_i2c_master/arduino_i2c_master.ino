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

void loop() {
  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(G_ADDRESS);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(1000);

  read3bytesState();
  delay(1000);

  Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  Wire.write(G_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();
  delay(1000);

  read3bytesState();
  delay(1000);
}
