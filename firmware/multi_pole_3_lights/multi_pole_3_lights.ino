/*
 * Xpark Lab - MOSFET-Based Smart Street Light
 * Three-pole adaptive controller for Arduino UNO
 *
 * Shared LDR: A0
 * FC-51 IR sensors: D2, D3, D4
 * MOSFET gate PWM: D9, D10, D11
 *
 * Research • Design • Prototype • Develop
 * Bashundhara R/A, Dhaka, Bangladesh
 */

#include <Arduino.h>

const uint8_t POLE_COUNT = 3;
const uint8_t LDR_PIN = A0;
const uint8_t IR_PINS[POLE_COUNT] = {2, 3, 4};
const uint8_t MOSFET_PINS[POLE_COUNT] = {9, 10, 11};

#define ENABLE_LOOKAHEAD true

const int DARK_THRESHOLD = 400;
const int BRIGHT_THRESHOLD = 450;
const uint8_t DIM_BRIGHTNESS = 64;
const uint8_t FULL_BRIGHTNESS = 255;
const uint8_t FADE_STEP = 5;
const unsigned long FADE_INTERVAL = 20;
const unsigned long HOLD_DURATION = 5000;
const unsigned long STATUS_INTERVAL = 500;

// Standard FC-51 modules output LOW when an obstacle is detected.
// Set false when testing an inverted-output or normally-high module.
const bool IR_ACTIVE_LOW = true;

struct PoleState {
  uint8_t currentBrightness;
  uint8_t targetBrightness;
  unsigned long lastDetectTime;
};

PoleState poles[POLE_COUNT];
bool isNight = false;
unsigned long lastFadeTime = 0;
unsigned long lastStatusTime = 0;

bool readIrDetected(uint8_t pin) {
  bool rawLow = (digitalRead(pin) == LOW);
  return IR_ACTIVE_LOW ? rawLow : !rawLow;
}

void updateAmbientMode(int ldrValue) {
  if (!isNight && ldrValue < DARK_THRESHOLD) {
    isNight = true;
  } else if (isNight && ldrValue > BRIGHT_THRESHOLD) {
    isNight = false;
  }
}

void updatePoleTargets(const bool detected[POLE_COUNT], unsigned long now) {
  if (!isNight) {
    for (uint8_t i = 0; i < POLE_COUNT; i++) {
      poles[i].targetBrightness = 0;
    }
    return;
  }

  bool detectedNow[POLE_COUNT];
  for (uint8_t i = 0; i < POLE_COUNT; i++) {
    detectedNow[i] = detected[i];
    if (detectedNow[i]) {
      poles[i].lastDetectTime = now;
    }

    bool heldDetection =
        (now - poles[i].lastDetectTime <= HOLD_DURATION);
    poles[i].targetBrightness =
        heldDetection ? FULL_BRIGHTNESS : DIM_BRIGHTNESS;
  }

#if ENABLE_LOOKAHEAD
  // Brighten the next pole while a detection is held.
  // Change i + 1 to i - 1 if traffic moves in the opposite direction.
  for (uint8_t i = 0; i < POLE_COUNT; i++) {
    if (detectedNow[i] && i + 1 < POLE_COUNT) {
      poles[i + 1].targetBrightness = FULL_BRIGHTNESS;
    }
  }
#endif
}

void fadePoles(unsigned long now) {
  if (now - lastFadeTime < FADE_INTERVAL) {
    return;
  }
  lastFadeTime = now;

  for (uint8_t i = 0; i < POLE_COUNT; i++) {
    PoleState &pole = poles[i];
    if (pole.currentBrightness < pole.targetBrightness) {
      uint16_t nextBrightness =
          (uint16_t)pole.currentBrightness + FADE_STEP;
      pole.currentBrightness =
          (nextBrightness > pole.targetBrightness)
              ? pole.targetBrightness
              : (uint8_t)nextBrightness;
    } else if (pole.currentBrightness > pole.targetBrightness) {
      int nextBrightness = (int)pole.currentBrightness - FADE_STEP;
      pole.currentBrightness =
          (nextBrightness < pole.targetBrightness)
              ? pole.targetBrightness
              : (uint8_t)nextBrightness;
    }
    analogWrite(MOSFET_PINS[i], pole.currentBrightness);
  }
}

void printStatus(int ldrValue, const bool detected[POLE_COUNT],
                 unsigned long now) {
  if (now - lastStatusTime < STATUS_INTERVAL) {
    return;
  }
  lastStatusTime = now;

  Serial.print("LDR:");
  Serial.print(ldrValue);
  Serial.print(" Mode:");
  Serial.print(isNight ? "NIGHT" : "DAY");

  for (uint8_t i = 0; i < POLE_COUNT; i++) {
    Serial.print(" | P");
    Serial.print(i + 1);
    Serial.print(":");
    Serial.print(detected[i] ? "IR+" : "IR-");
    Serial.print(":");
    Serial.print(poles[i].currentBrightness);
  }
  Serial.println();
}

void setup() {
  Serial.begin(9600);

  for (uint8_t i = 0; i < POLE_COUNT; i++) {
    pinMode(IR_PINS[i], INPUT);
    pinMode(MOSFET_PINS[i], OUTPUT);
    analogWrite(MOSFET_PINS[i], 0);
    poles[i].currentBrightness = 0;
    poles[i].targetBrightness = 0;
    poles[i].lastDetectTime = millis() - HOLD_DURATION - 1;
  }

  Serial.println("Xpark Lab Smart Street Light - Three Poles");
  Serial.println("P1/P2/P3 = IR state and PWM brightness");
}

void loop() {
  const unsigned long now = millis();
  const int ldrValue = analogRead(LDR_PIN);
  bool detected[POLE_COUNT];

  for (uint8_t i = 0; i < POLE_COUNT; i++) {
    detected[i] = readIrDetected(IR_PINS[i]);
  }

  updateAmbientMode(ldrValue);
  updatePoleTargets(detected, now);
  fadePoles(now);
  printStatus(ldrValue, detected, now);
}
