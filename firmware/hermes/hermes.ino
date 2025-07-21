#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define MPU_ADDR 0x68
#define OLED_ADDR 0x3

float rotationZ = 0;
unsigned long lastTime = 0;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  delay(100);

  initMPU();
}

void loop() {
  readGyro();

  displayRot();

  delay(200);
}

void initMPU() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);  //sleep mode address
  Wire.write(0x00);  //hex to start sensor

  Wire.write(0x1B);  //calibration address
  Wire.write(0x00);  //hex for +/- 250 range
  Wire.endTransmission();
}

void readGyro() {
  int16_t gyroX, gyroY, gyroZ;
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x43);  //GYRO_XOUT_H
  Wire.endTransmission(false);

  //request 6 bytes (1 per register)
  Wire.requestFrom(MPU_ADDR, 6);

  gyroX = (Wire.read() << 8) | Wire.read();
  gyroY = (Wire.read() << 8) | Wire.read();
  gyroZ = (Wire.read() << 8) | Wire.read();

  //converting to degrees/second
  float gyroRate = gyroZ / 131.0;

  //calculating time elapsed
  unsigned long now = millis();
  float dt = (now - lastTime) / 1000.0;

  //math to get total rotation
  rotationZ += gyroRate * dt;
  lastTime = now;
}

void displayRot() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Current Rotation:");
  display.setCursor(0, 16);
  display.print(rotationZ);
}