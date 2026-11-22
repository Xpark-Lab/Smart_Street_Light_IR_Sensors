# Xpark Lab Testing and Calibration Guide

**Xpark Lab — Research • Design • Prototype • Develop**
Bashundhara R/A, Dhaka, Bangladesh

## 1. Before Power-Up

1. Verify the Arduino UNO board and serial port in the Arduino IDE.
2. Connect the digital LDR module VCC/GND to `5V/GND` and DO to `D2`.
3. Connect IR sensor 1, 2, and 3 OUT pins to `D3`, `D4`, and `D5`.
4. Connect MOSFET gates to `D9`, `D10`, and `D11` through separate 100 Ω resistors.
5. Add a separate 10 kΩ pull-down between every Gate pin and common GND.
6. Use low-voltage LEDs and 220 Ω series resistors for initial testing.

## 2. Digital Sensor Test

1. Open `test/sensor_test/sensor_test.ino`.
2. Select **Arduino UNO** and upload the sketch.
3. Open Serial Monitor and set the baud rate to **9600**.
4. Cover the digital LDR module and confirm `LDR: Dark`.
5. Shine light on the module and confirm `LDR: Light`.
6. Wave an object in front of IR 1, IR 2, and IR 3 in turn.
7. Confirm the matching channel shows `Object`, then `No Object`.

If LDR results are reversed, set:

```cpp
const bool LDR_ACTIVE_HIGH_IN_DARK = false;
```

Standard IR modules are active LOW. If an IR sensor behaves in reverse, set:

```cpp
const bool IR_ACTIVE_LOW = false;
```

Example sensor-test output:

```text
LDR: Dark | IR1: Object | IR2: No Object | IR3: No Object
```

## 3. Main Firmware Test

1. Disconnect the LED supply.
2. Complete all three MOSFET, gate resistor, and pull-down connections.
3. For each LED, connect `5V → 220 Ω → LED anode → LED cathode → MOSFET Drain`.
4. Connect every MOSFET Source to common GND.
5. Upload `firmware/multi_pole_3_lights/multi_pole_3_lights.ino`.
6. Open Serial Monitor at **9600 baud**.

Example main-firmware output:

```text
LDR: Dark | Ch1: ON | Ch2: OFF | Ch3: OFF
```

## 4. Functional Test

### Startup OFF test

1. With no Arduino or sensor input active, verify all three channels report `OFF`.
2. Verify that all three LED loads remain off.

### Dark and object test

1. Cover the digital LDR until Serial Monitor reports `Dark`.
2. Wave an object across IR 1 only.
3. Confirm `Ch1: ON`; confirm `Ch2: OFF` and `Ch3: OFF`.
4. Repeat with IR 2 and IR 3.

### Light test

1. Shine light on the digital LDR until it reports `Light`.
2. Detect objects with every IR sensor.
3. Confirm `Ch1`, `Ch2`, and `Ch3` remain `OFF`.

## 5. Serial Troubleshooting

| Symptom | Check |
|---|---|
| No Serial output | USB connection, COM port, and 9600 baud |
| LDR stuck on one state | DO to `D2`, VCC/GND wiring, inversion constant |
| IR never detects | 5 V supply, OUT pin, potentiometer, inversion constant |
| LED stays off | Gate wiring, 100 Ω resistor, 10 kΩ pull-down, 220 Ω LED resistor |
| LED stays on | Sensor stuck active, Gate stuck HIGH, wiring at `D9/D10/D11` |
| One channel only is wrong | Check that channel's IR, MOSFET, gate resistor, pull-down, and LED |
| MOSFET hot | Stop operation, verify Source/Gate/Drain pin order and load current |

## 6. Handover Checklist

- [ ] Digital LDR reports `Dark` and `Light` correctly.
- [ ] IR 1 reports `Object` on `D3`.
- [ ] IR 2 reports `Object` on `D4`.
- [ ] IR 3 reports `Object` on `D5`.
- [ ] All three MOSFET gates use 100 Ω resistors.
- [ ] All three gates use 10 kΩ pull-down resistors.
- [ ] Every LED uses a 220 Ω series resistor.
- [ ] Startup writes LOW to `D9`, `D10`, and `D11`.
- [ ] Serial Monitor reports at 9600 baud.
- [ ] Only dark plus a matching detected object turns a channel ON.
