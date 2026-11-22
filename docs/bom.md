# Xpark Lab Bill of Materials

**Project:** MOSFET-Based Smart Street Light Control System Using IR Sensors
**Controller:** Arduino UNO
**Document owner:** Xpark Lab
**Location:** Bashundhara R/A, Dhaka, Bangladesh

Quantities below support the full 3-pole prototype and include useful spares.

| Component | Quantity | Xpark Lab Ref. / Part | Notes |
|---|---:|---|---|
| Arduino UNO | 1 pc | — | Main controller |
| IRF540N N-channel MOSFET | 3 pcs | — | Pin 1 Gate, Pin 2 Drain, Pin 3 Source |
| FC-51 IR obstacle sensor module | 3 pcs | — | Digital OUT, active LOW when object detected |
| Digital LDR sensor module | 1 pc | — | 3-pin VCC/GND/DO; DO active HIGH in dark |
| 5 mm LED | 3 pcs | — | One load per MOSFET channel |
| 100 Ω resistor | 3 pcs | — | Gate resistor, one per MOSFET |
| 10 kΩ resistor | 3 pcs | — | Gate-to-GND pull-down, one per MOSFET |
| 220 Ω resistor | 3 pcs | — | LED series resistor, one per LED |
| 47 Ω resistor | Optional | — | Additional low-load experiment resistor |
| 1 kΩ resistor | Optional | — | General-purpose |
| 10 kΩ resistor | 1 spare | — | Optional spare/generic |
| Aluminum heatsink | 3 pcs | — | MOSFET thermal management |
| 14500 3.7 V 1300 mAh rechargeable lithium battery | 3 pcs | — | DC experiment supply only |
| 14500 battery case | 1 pc | — | Holds 14500 cells |
| Jumper wires M-M | 1 set | — | Arduino and breadboard |
| Jumper wires F-F | 1 set | — | Sensor/module connections |
| Jumper wires M-M / F-F / M-F | 1 set each | — | Breadboard and hardware interface |
| Breadboard | 1 pc | — | Prototyping |
| 1N5408 diode | Optional | — | Flyback diode for inductive DC loads only |

> **Assembly note:** The finalized 3-pole implementation uses exactly 3 IRF540N devices, 3 LEDs, 3x 100 Ω gate resistors, 3x 10 kΩ gate pull-down resistors, and 3x 220 Ω LED series resistors.
