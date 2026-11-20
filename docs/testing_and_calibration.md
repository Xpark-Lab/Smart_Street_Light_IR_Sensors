# Xpark Lab Testing and Calibration Guide

**Xpark Lab — Research • Design • Prototype • Develop**
Bashundhara R/A, Dhaka, Bangladesh

## 1. Before Power-Up

1. Verify Arduino UNO board selection and serial port.
2. Confirm the LDR divider is wired to `A0`, ground, and 5 V.
3. Confirm FC-51 sensor outputs match `D2/D3/D4`.
4. Confirm MOSFET gates match `D9/D10/D11`.
5. Add a 100–220 Ω gate resistor and 10 kΩ gate pull-down.
6. Start with low-voltage LED loads, not mains or unknown lamp loads.

## 2. Upload and Serial Monitor

1. Open `firmware/single_pole/single_pole.ino` or `firmware/multi_pole_3_lights/multi_pole_3_lights.ino`.
2. Select **Arduino UNO**.
3. Upload the sketch.
4. Open Serial Monitor.
5. Set baud rate to **9600**.
6. Confirm periodic `LDR`, `Mode`, `IR`, and `Brightness` output.

For the 3-pole firmware, status is compact:

```text
LDR:390 Mode:NIGHT | P1:IR-:64 | P2:IR+:255 | P3:IR-:64
```

## 3. LDR Calibration

The default thresholds are:

- Night: ADC value below `400`
- Day: ADC value above `450`
- Dead band: `400–450`

The dead band prevents rapid toggling while light levels change.

### Calibration procedure

1. In normal daytime conditions, cover the LDR with the hand and record Serial Monitor readings.
2. In fully dark conditions, record the lowest stable reading.
3. Shine a flashlight directly over the LDR and record the bright reading.
4. Adjust `DARK_THRESHOLD` and `BRIGHT_THRESHOLD` only if the default values do not match the installation.
5. Keep the dark threshold lower than the bright threshold.
6. Use a separation of at least 30–50 ADC counts when the environment has light flicker.

Example for a stable outdoor installation:

```cpp
// Start at defaults; increase only after outdoor logging.
const int DARK_THRESHOLD = 400;
const int BRIGHT_THRESHOLD = 450;
```

## 4. FC-51 Calibration

1. Power the sensor at 3.3–5 V.
2. Rotate the blue potentiometer counter-clockwise for a shorter range.
3. Rotate it clockwise for a longer range.
4. Begin with 10–15 cm for indoor bench testing.
5. Cover the sensor face and wave a hand through the target range.
6. Confirm `IR: DETECTED` in the single-pole status or `IR+` in the 3-pole status.
7. Remove the object and confirm `IR: CLEAR` or `IR-`.

Standard FC-51 modules are active LOW. If the status is inverted, set:

```cpp
const bool IR_ACTIVE_LOW = false;
```

Mount sensors low enough to detect the intended object but shielded from direct lamp glare.

## 5. MOSFET and Lamp Test

1. Disconnect the lamp supply.
2. Connect the MOSFET source to common ground.
3. Connect the gate to PWM through 100–220 Ω.
4. Add the 10 kΩ gate pull-down.
5. Connect a low-voltage LED or lamp rated for the supply.
6. Add a 150 Ω series resistor for a typical 5 V LED; derive the resistor from the LED datasheet for other parts.
7. Apply power and observe the standby value of 64 at night.
8. Measure MOSFET temperature during continuous operation.
9. Add an aluminum heatsink and improve ventilation if temperature rises excessively.

## 6. Required Functional Test

### LDR turns lamp ON in dim mode

1. Place the board in a dark area.
2. Wait for the LDR reading to fall below 400.
3. Confirm `Mode: NIGHT`.
4. Confirm PWM brightness settles near `64`.

### Hand wave triggers full brightness

1. While nighttime mode is active, wave a hand in front of the FC-51 sensor.
2. Confirm `IR: DETECTED`.
3. Confirm brightness ramps to `255`.
4. Keep the object clear for at least 5 seconds.
5. Confirm brightness fades back toward `64`.

### Flashlight turns lamp OFF

1. Shine a flashlight on the LDR.
2. Confirm the reading rises above 450.
3. Confirm `Mode: DAY`.
4. Confirm PWM brightness settles at `0`.

### Three-pole follow test

1. Illuminate the sensor for Pole 1.
2. Confirm Pole 1 ramps to full.
3. If look-ahead is enabled, confirm the next pole also reaches full.
4. Move detections toward Pole 2 and Pole 3.
5. Confirm unheld poles return from full to 64 standby.

## 7. Serial Troubleshooting

| Symptom | Check |
|---|---|
| No Serial output | Correct COM port and 9600 baud |
| LDR stuck near 1023 | Sensor divider wiring or shorted signal |
| LDR stuck near 0 | Wrong divider orientation or 10 kΩ location |
| IR never detects | 5 V supply, OUT pin, potentiometer, inversion setting |
| Lamp off in every mode | MOSFET gate, ground, PWM pin, load supply |
| Lamp stuck full | Sensor stuck LOW; gate switching; target hold logic |
| Flickering at dusk | Increase hysteresis gap or shield LDR |
| MOSFET hot | Add heatsink, verify IRLZ44N/IRF540N suitability, load current |

## 8. Handover Checklist

- [ ] Day mode turns PWM off.
- [ ] Night mode provides dim standby.
- [ ] IR detection reaches full brightness.
- [ ] Hold duration lasts 5 seconds after final detection.
- [ ] Fade behavior is smooth.
- [ ] Serial Monitor reports at 9600 baud.
- [ ] All grounds are common.
- [ ] Lamp supply and enclosure are safely fused.
- [ ] MOSFET devices are heatsinked when required.
