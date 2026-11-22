/*
 * Xpark Lab - MOSFET-Based Smart Street Light
 * Single-pole controller for Arduino UNO
 *
 * Digital LDR DO: D2
 * FC-51 IR sensor: D3
 * MOSFET gate PWM: D9
 *
 * Research • Design • Prototype • Develop
 * Bashundhara R/A, Dhaka, Bangladesh
 */

#include <Arduino.h>

const uint8_t LDR_PIN = 2;
const uint8_t IR_SENSOR_PIN = 3;
const uint8_t MOSFET_PIN = 9;

const unsigned long STATUS_INTERVAL = 250;

const bool LDR_ACTIVE_HIGH_IN_DARK = true;
const bool IR_ACTIVE_LOW = true;

unsigned long lastStatusTime = 0;

bool readIsDark() {
  const bool rawHigh = (digitalRead(LDR_PIN) == HIGH);
  return LDR_ACTIVE_HIGH_IN_DARK ? rawHigh : !rawHigh;
}

bool readIsObjectDetected() {
  const bool rawLow = (digitalRead(IR_SENSOR_PIN) == LOW);
  return IR_ACTIVE_LOW ? rawLow : !rawLow;
}

void printStatus(bool isDark, bool isOn, unsigned long now) {
  if (now - lastStatusTime < STATUS_INTERVAL) {
    return;
  }
  lastStatusTime = now;

  Serial.print("LDR: ");
  Serial.print(isDark ? "Dark" : "Light");
  Serial.print(" | Ch1: ");
  Serial.println(isOn ? "ON" : "OFF");
}

void setup() {
  Serial.begin(9600);
  pinMode(LDR_PIN, INPUT);
  pinMode(IR_SENSOR_PIN, INPUT);
  pinMode(MOSFET_PIN, OUTPUT);
  digitalWrite(MOSFET_PIN, LOW);

  Serial.println("Xpark Lab Smart Street Light - Single Pole");
  Serial.println("LDR: Dark/Light | Ch1: ON/OFF");
}

void loop() {
  const unsigned long now = millis();
  const bool isDark = readIsDark();
  const bool isObjectDetected = readIsObjectDetected();
  const bool isOn = isDark && isObjectDetected;

  digitalWrite(MOSFET_PIN, isOn ? HIGH : LOW);
  printStatus(isDark, isOn, now);
}
