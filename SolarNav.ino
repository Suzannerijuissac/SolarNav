#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <QMC5883LCompass.h>
#include <DHT.h>
#include <stdio.h>

// -------- OLED (RAM SAFE 128x32) --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// -------- SENSORS --------
#define DHTTYPE DHT22
#define DHTPIN1 2
#define DHTPIN2 3
#define LDR_LEFT A0
#define LDR_RIGHT A1

QMC5883LCompass compass;
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

// -------- TIME --------
int inputHour = 14;
int inputMinute = 30;

// -------- NON-BLOCKING SERIAL TIME INPUT --------
void updateTimeFromSerial() {
  static char buffer[12];
  static byte index = 0;

  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\r') continue;   // ignore carriage return

    if (c == '\n') {
      buffer[index] = '\0';

      int h, m;
      if (sscanf(buffer, "%d %d", &h, &m) == 2) {
        if (h >= 0 && h <= 23 && m >= 0 && m <= 59) {
          inputHour = h;
          inputMinute = m;
        }
      }

      index = 0;
    }
    else {
      if (index < sizeof(buffer) - 1) {
        buffer[index++] = c;
      }
    }
  }
}

// -------- UTILITIES --------
float normalizeValue(float value, float minVal, float maxVal) {
  if (value < minVal) value = minVal;
  if (value > maxVal) value = maxVal;
  return (value - minVal) * 100.0 / (maxVal - minVal);
}

float normalizeAngle(float angle) {
  while (angle < 0) angle += 360;
  while (angle >= 360) angle -= 360;
  return angle;
}

float angleDifference(float a, float b) {
  float diff = abs(a - b);
  if (diff > 180) diff = 360 - diff;
  return diff;
}

float calculateSunAngle(int hour, int minute) {
  float t = hour + minute / 60.0;
  if (t < 6 || t > 18) return -1;
  return normalizeAngle(90 + (t - 6) * 15);
}

void setup() {

  Serial.begin(9600);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);  // stop if OLED fails
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(25, 10);
  display.println("SOLARNAV");
  display.display();
  delay(1000);

  compass.init();
  dht1.begin();
  dht2.begin();

  Serial.println("Type time as: HH MM");
}

void loop() {

  updateTimeFromSerial();

  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate < 800)
    return;

  lastUpdate = millis();

  float sunAngle = calculateSunAngle(inputHour, inputMinute);

  compass.read();
  int heading = compass.getAzimuth();

  char recommendation[16];

  if (sunAngle == -1) {
    strcpy(recommendation, "NIGHT");
  } else {
    float leftAngle = normalizeAngle(heading - 90);
    float rightAngle = normalizeAngle(heading + 90);

    float sunImpactLeft =
        100 - normalizeValue(angleDifference(leftAngle, sunAngle), 0, 180);

    float sunImpactRight =
        100 - normalizeValue(angleDifference(rightAngle, sunAngle), 0, 180);

    float tempLeft = dht1.readTemperature();
    float tempRight = dht2.readTemperature();
    float humLeft = dht1.readHumidity();
    float humRight = dht2.readHumidity();

    int ldrLeft = analogRead(LDR_LEFT);
    int ldrRight = analogRead(LDR_RIGHT);

    float scoreLeft =
        normalizeValue(tempLeft, 20, 50) * 0.35 +
        normalizeValue(humLeft, 30, 90) * 0.15 +
        normalizeValue(ldrLeft, 0, 1023) * 0.30 +
        sunImpactLeft * 0.20;

    float scoreRight =
        normalizeValue(tempRight, 20, 50) * 0.35 +
        normalizeValue(humRight, 30, 90) * 0.15 +
        normalizeValue(ldrRight, 0, 1023) * 0.30 +
        sunImpactRight * 0.20;

    if (scoreLeft - scoreRight > 5)
      strcpy(recommendation, "RIGHT");
    else if (scoreRight - scoreLeft > 5)
      strcpy(recommendation, "LEFT");
    else
      strcpy(recommendation, "SIMILAR");
  }

  // -------- OLED DISPLAY --------
  display.clearDisplay();

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("T:");
  display.print(inputHour);
  display.print(":");
  if (inputMinute < 10) display.print("0");
  display.println(inputMinute);

  display.print("H:");
  display.print(heading);
  display.print(" S:");
  display.println((int)sunAngle);

  display.setTextSize(2);
  display.setCursor(25, 16);
  display.println(recommendation);

  display.display();
}