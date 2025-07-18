#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MPU_ADDR 0x68
#define GYRO_CONFIG 0x1B
#define PWR_MGMT_1 0x6B
#define GYRO_XOUT_H 0x43

void setup() {
  Wire.begin();
  Serial.begin(115200);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  delay(100);
}

void loop() {
  int16_t gyroX, gyroY, gyroZ;

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(GYRO_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6);

  if (Wire.available() == 6) {
    gyroX = Wire.read() << 8 | Wire.read();
    gyroY = Wire.read() << 8 | Wire.read();
    gyroZ = Wire.read() << 8 | Wire.read();
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("MPU-6500 Gyro:");
  display.setCursor(0, 16);
  display.print("X: "); display.print(gyroX);
  display.setCursor(0, 28);
  display.print("Y: "); display.print(gyroY);
  display.setCursor(0, 40);
  display.print("Z: "); display.print(gyroZ);
  display.display();

  delay(200);
}
