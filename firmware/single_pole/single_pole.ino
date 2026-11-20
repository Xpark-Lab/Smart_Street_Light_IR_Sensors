/*
 * Xpark Lab - MOSFET-Based Smart Street Light
 * Single-pole controller for Arduino UNO
 *
 * LDR: A0
 * FC-51 IR sensor: D2
 * MOSFET gate PWM: D9
 *
 * Research • Design • Prototype • Develop
 * Bashundhara R/A, Dhaka, Bangladesh
 */

#include <Arduino.h>

const uint8_t LDR_PIN = A0;
const uint8_t IR_SENSOR_PIN = 2;
const uint8_t MOSFET_PIN = 9;

const int DARK_THRESHOLD = 400;
const int BRIGHT_THRESHOLD = 450;
const uint8_t DIM_BRIGHTNESS = 64;
const uint8_t FULL_BRIGHTNESS = 255;
const uint8_t FADE_STEP = 5;
const unsigned long FADE_INTERVAL = 20;
const unsigned long HOLD_DURATION = 5000;
const unsigned long STATUS_INTERVAL = 250;

// Standard FC-51 modules output LOW when an obstacle is detected.
// Set false when testing an inverted-output or normally-high module.
const bool IR_ACTIVE_LOW = true;

bool isNight = false;
uint8_t currentBrightness = 0;
uint8_t targetBrightness = 0;
unsigned long lastDetectTime = 0;
unsigned long lastFadeTime = 0;
unsigned long lastStatusTime = 0;

bool readIrDetected() {
  bool rawLow = (digitalRead(IR_SENSOR_PIN) == LOW);
  return IR_ACTIVE_LOW ? rawLow : !rawLow;
}

void updateAmbientMode(int ldrValue) {
  if (!isNight && ldrValue < DARK_THRESHOLD) {
    isNight = true;
  } else if (isNight && ldrValue > BRIGHT_THRESHOLD) {
    isNight = false;
  }
}

void updateFadeTarget(bool irDetected, unsigned long now) {
  if (!isNight) {
    targetBrightness = 0;
    return;
  }

  if (irDetected) {
    lastDetectTime = now;
  }

  targetBrightness =
      (now - lastDetectTime <= HOLD_DURATION) ? FULL_BRIGHTNESS : DIM_BRIGHTNESS;
}

void fadeLamp(unsigned long now) {
  if (now - lastFadeTime < FADE_INTERVAL) {
    return;
  }
  lastFadeTime = now;

  if (currentBrightness < targetBrightness) {
    uint16_t nextBrightness = (uint16_t)currentBrightness + FADE_STEP;
    currentBrightness = (nextBrightness > targetBrightness)
                            ? targetBrightness
                            : (uint8_t)nextBrightness;
  } else if (currentBrightness > targetBrightness) {
    int nextBrightness = (int)currentBrightness - FADE_STEP;
    currentBrightness = (nextBrightness < targetBrightness)
                            ? targetBrightness
                            : (uint8_t)nextBrightness;
  }

  analogWrite(MOSFET_PIN, currentBrightness);
}

void printStatus(int ldrValue, bool irDetected, unsigned long now) {
  if (now - lastStatusTime < STATUS_INTERVAL) {
    return;
  }
  lastStatusTime = now;

  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print(" | Mode: ");
  Serial.print(isNight ? "NIGHT" : "DAY");
  Serial.print(" | IR: ");
  Serial.print(irDetected ? "DETECTED" : "CLEAR");
  Serial.print(" | Brightness: ");
  Serial.println(currentBrightness);
}

void setup() {
  Serial.begin(9600);
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(MOSFET_PIN, OUTPUT);
  analogWrite(MOSFET_PIN, 0);
  lastDetectTime = millis() - HOLD_DURATION - 1;

  Serial.println("Xpark Lab Smart Street Light - Single Pole");
  Serial.println("Mode: DAY/NIGHT | IR: DETECTED/CLEAR");
}

void loop() {
  const unsigned long now = millis();
  const int ldrValue = analogRead(LDR_PIN);
  const bool irDetected = readIrDetected();

  updateAmbientMode(ldrValue);
  updateFadeTarget(irDetected, now);
  fadeLamp(now);
  printStatus(ldrValue, irDetected, now);
}
