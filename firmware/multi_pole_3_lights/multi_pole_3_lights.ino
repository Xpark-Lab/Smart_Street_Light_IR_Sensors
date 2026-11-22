/*
 * Xpark Lab - MOSFET-Based Smart Street Light
 * Three-pole digital sensor controller for Arduino UNO
 *
 * Digital LDR DO: D2 (active HIGH in darkness)
 * FC-51 IR sensors: D3, D4, D5 (active LOW when detected)
 * MOSFET gate PWM: D9, D10, D11
 *
 * Research • Design • Prototype • Develop
 * Bashundhara R/A, Dhaka, Bangladesh
 */

#include <Arduino.h>

const uint8_t POLE_COUNT = 3;
const uint8_t LDR_PIN = 2;
const uint8_t IR_PINS[POLE_COUNT] = {3, 4, 5};
const uint8_t MOSFET_PINS[POLE_COUNT] = {9, 10, 11};

// Set to false if the digital LDR module's DO pin is inverted.
const bool LDR_ACTIVE_HIGH_IN_DARK = true;
const unsigned long STATUS_INTERVAL = 500;
// Standard IR modules output LOW when an obstacle is detected.
const bool IR_ACTIVE_LOW = true;

unsigned long lastStatusTime = 0;

bool readIsDark() {
  const bool rawHigh = (digitalRead(LDR_PIN) == HIGH);
  return LDR_ACTIVE_HIGH_IN_DARK ? rawHigh : !rawHigh;
}

bool readIsObjectDetected(uint8_t pin) {
  const bool rawLow = (digitalRead(pin) == LOW);
  return IR_ACTIVE_LOW ? rawLow : !rawLow;
}

void printStatus(bool isDark, const bool isOn[POLE_COUNT],
                 unsigned long now) {
  if (now - lastStatusTime < STATUS_INTERVAL) {
    return;
  }
  lastStatusTime = now;

  Serial.print("LDR: ");
  Serial.print(isDark ? "Dark" : "Light");

  for (uint8_t i = 0; i < POLE_COUNT; i++) {
    Serial.print(" | Ch");
    Serial.print(i + 1);
    Serial.print(":");
    Serial.print(isOn[i] ? "ON" : "OFF");
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  pinMode(LDR_PIN, INPUT);

  for (uint8_t i = 0; i < POLE_COUNT; i++) {
    pinMode(IR_PINS[i], INPUT);
    pinMode(MOSFET_PINS[i], OUTPUT);
    digitalWrite(MOSFET_PINS[i], LOW);
  }

  Serial.println("Xpark Lab Smart Street Light - Three Poles");
  Serial.println("LDR: Dark/Light | Ch1/Ch2/Ch3: ON/OFF");
}

void loop() {
  const unsigned long now = millis();
  const bool isDark = readIsDark();
  bool isOn[POLE_COUNT];

  for (uint8_t i = 0; i < POLE_COUNT; i++) {
    const bool isObjectDetected = readIsObjectDetected(IR_PINS[i]);
    isOn[i] = isDark && isObjectDetected;
    digitalWrite(MOSFET_PINS[i], isOn[i] ? HIGH : LOW);
  }

  printStatus(isDark, isOn, now);
}
