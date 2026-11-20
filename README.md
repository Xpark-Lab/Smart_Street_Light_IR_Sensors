# Xpark Lab | MOSFET-Based Smart Street Light Control System

![Xpark Lab](https://img.shields.io/badge/Xpark%20Lab-Research%20%E2%80%A2%20Design%20%E2%80%A2%20Prototype%20%E2%80%A2%20Develop-blue)

**Research • Design • Prototype • Develop**
**Bashundhara R/A, Dhaka, Bangladesh**

This open-source repository contains production-minded Arduino firmware and maker documentation for a low-cost, adaptive street-light system. The system uses an LDR for day/night control and FC-51 infrared sensors to detect vehicles or pedestrians. An Arduino UNO drives logic-level N-channel MOSFETs with PWM for dim, full, and smoothly fading output.

## Prototype Hardware

![MOSFET-Based Smart Street Light Hardware Prototype](docs/images/MOSFET_Based_Smart_Street_Light_IR_Sensors_hardware.png)

*Xpark Lab hardware prototype — MOSFET-based smart street light with IR sensing.*
[Download the original hardware diagram (PDF)](docs/images/MOSFET_Based_Smart_Street_Light_IR_Sensors%20hardwear.pdf)

## Project Variants

| Variant | Coverage | IR sensors | MOSFET/PWM outputs | Sketch |
|---|---|---:|---:|---|
| Single pole | One lamp | 1 | 1 | `firmware/single_pole/single_pole.ino` |
| Three-pole adaptive | One road section | 3 | 3 | `firmware/multi_pole_3_lights/multi_pole_3_lights.ino` |

## Operating Behavior

### Single pole

- **Day:** PWM is 0; the lamp is fully off.
- **Night, no traffic:** PWM remains at 64 (approximately 25%).
- **Night, detection:** The lamp ramps from standby to PWM 255, remains full for 5 seconds after the last detection, then fades back to 64.
- Dusk/dawn sensing uses hysteresis: night below 400 ADC and day above 450 ADC.
- Fading runs every 20 ms in the main loop, independent of Serial output.

### Three-pole adaptive system

- Each pole has its own brightness, fade target, and 5-second detection hold time.
- A detected pole receives full brightness.
- With `ENABLE_LOOKAHEAD true`, the next pole in the direction of travel is also illuminated.
- Adjacent poles fade to standby after their look-ahead hold expires.
- All lamps switch off during daytime.

## Hardware Summary

| Item | Recommendation |
|---|---|
| Controller | Arduino UNO |
| Ambient sensor | 10 kΩ LDR voltage divider on `A0` |
| Motion/obstacle sensor | FC-51 IR obstacle sensor at 5 V |
| Lamp switch | IRLZ44N logic-level MOSFET recommended; IRF540N compatible for low-current LED loads |
| Lamp gate interface | 100–220 Ω gate resistor and 10 kΩ gate-to-source pull-down |

The lamp is switched on the low side. The MOSFET source connects to ground, drain connects to the lamp negative lead, and gate receives PWM from the Arduino UNO. The lamp positive lead connects to a supply appropriate for the lamp. Never connect mains AC directly to this circuit.

## Quick Start

1. Install [Arduino IDE](https://www.arduino.cc/en/software).
2. Open the desired sketch.
3. Select **Arduino UNO** under **Tools → Board**.
4. Select the board's serial port.
5. Upload and open Serial Monitor at **9600 baud**.
6. Verify LDR and IR operation using `docs/testing_and_calibration.md`.
7. Complete MOSFET and lamp wiring only after the control electronics are de-energized.

No third-party Arduino libraries are required.

## Safety Notice

- This repository demonstrates low-voltage DC LED loads only.
- Use an appropriately rated DC supply and fuse for the lamp load.
- Do not work on mains AC wiring unless you are a qualified professional.
- Disconnect power before changing MOSFET, gate, or lamp connections.
- Use a heatsink when MOSFET dissipation warrants it.
- Add an appropriate flyback diode only to inductive DC loads; use a 1N5408 where compatible.
- Verify MOSFET polarity, source ground, and heat dissipation before continuous operation.

## Pin Assignments

### Single pole

| Signal | Arduino UNO pin |
|---|---|
| LDR divider | `A0` |
| FC-51 OUT | `D2` |
| MOSFET gate | `D9` PWM |

### Three-pole

| Pole | FC-51 OUT | MOSFET gate |
|---|---:|---:|
| Pole 1 | `D2` | `D9` PWM |
| Pole 2 | `D3` | `D10` PWM |
| Pole 3 | `D4` | `D11` PWM |

Shared LDR: `A0`.

## Repository Guide

- `firmware/single_pole/` — one-pole implementation.
- `firmware/multi_pole_3_lights/` — three-pole adaptive implementation.
- `docs/wiring_guide.md` — pinouts, schematics, and assembly notes.
- `docs/bom.md` — Xpark Lab bill of materials.
- `docs/testing_and_calibration.md` — calibration and commissioning procedure.
- `docs/images/` — place schematics and build photos here.

## Applications

- Highways and rural roads
- University campuses
- Parks and pedestrian walkways
- Parking lots
- Industrial perimeters

## Future Improvements / IoT Upgrade

The firmware can evolve to solar charging, battery monitoring, data logging, remote fault alerts, or wireless scheduling. A future ESP32-based edition could add Wi-Fi dashboard and OTA support, but it is not the target controller for this release.

## Contributing

Contributions should preserve direct Arduino UNO compatibility, non-blocking behavior, and clear safety documentation. Open an issue or submit a pull request with tested hardware details and Serial Monitor evidence.

## License

MIT License. Copyright © Xpark Lab. See `LICENSE`.
