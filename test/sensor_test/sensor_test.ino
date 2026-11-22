/*
 * Xpark Lab - Digital sensor test
 * Arduino UNO
 *
 * LDR module DO: D2 (active HIGH in darkness)
 * IR modules OUT: D3, D4, D5 (active LOW when detected)
 *
 * Open Serial Monitor at 9600 baud.
 */

#include <Arduino.h>

const uint8_t LDR_PIN = 2;
const uint8_t IR_PINS[] = {3, 4, 5};
const uint8_t IR_COUNT = 3;

// Change to false if the digital LDR module reports the opposite state.
const bool LDR_ACTIVE_HIGH_IN_DARK = true;
// Standard IR modules report LOW when an object is detected.
const bool IR_ACTIVE_LOW = true;
const unsigned long PRINT_INTERVAL = 250;

unsigned long lastPrintTime = 0;

bool readIsDark() {
  const bool rawHigh = (digitalRead(LDR_PIN) == HIGH);
  return LDR_ACTIVE_HIGH_IN_DARK ? rawHigh : !rawHigh;
}

bool readIsObjectDetected(uint8_t pin) {
  const bool rawLow = (digitalRead(pin) == LOW);
  return IR_ACTIVE_LOW ? rawLow : !rawLow;
}

void setup() {
  Serial.begin(9600);
  pinMode(LDR_PIN, INPUT);

  for (uint8_t i = 0; i < IR_COUNT; i++) {
    pinMode(IR_PINS[i], INPUT);
  }

  Serial.println("Xpark Lab Sensor Test");
  Serial.println("LDR: Dark/Light | IR1/IR2/IR3: Object/No Object");
}

void loop() {
  const unsigned long now = millis();
  if (now - lastPrintTime < PRINT_INTERVAL) {
    return;
  }
  lastPrintTime = now;

  const bool isDark = readIsDark();
  Serial.print("LDR: ");
  Serial.print(isDark ? "Dark" : "Light");

  for (uint8_t i = 0; i < IR_COUNT; i++) {
    const bool isObjectDetected = readIsObjectDetected(IR_PINS[i]);
    Serial.print(" | IR");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(isObjectDetected ? "Object" : "No Object");
  }
  Serial.println();
}
