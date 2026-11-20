# Xpark Lab Bill of Materials

**Project:** MOSFET-Based Smart Street Light Control System Using IR Sensors
**Controller:** Arduino UNO
**Document owner:** Xpark Lab
**Location:** Bashundhara R/A, Dhaka, Bangladesh

Quantities below support the full 3-pole prototype and include useful spares.

| Component | Quantity | Xpark Lab Ref. / Part | Notes |
|---|---:|---|---|
| Arduino UNO | 1 pc | — | Main controller |
| IRF540N / IRLZ44N N-channel MOSFET | 6 pcs | — | IRLZ44N preferred for 5 V logic |
| FC-51 IR obstacle sensor | 6 pcs | — | 3.3–5 V, 2–30 cm adjustable range |
| LDR 5 mm | 4 pcs | — | Ambient sensing and spares |
| LDR sensor module | 1 pc | — | Optional module; bench sensor reference |
| 5 mm LED, Red | 5 pcs | — | Test load/status indicator |
| 5 mm LED, Blue | 5 pcs | — | Test load/status indicator |
| 5 mm LED, Green | 5 pcs | — | Test load/status indicator |
| 47 Ω resistor | 10 pcs | — | MOSFET/load experiment series resistor |
| 100 Ω resistor | 15 pcs | — | Gate resistor and low-load experiment resistor |
| 220 Ω resistor | 10 pcs | — | Gate resistor |
| 1 kΩ resistor | 10 pcs | — | General-purpose circuit resistor |
| 10 kΩ resistor | 10 pcs | — | LDR divider and gate pull-down |
| Aluminum heatsink | 3 pcs | — | MOSFET thermal management |
| 14500 3.7 V 1300 mAh rechargeable lithium battery | 3 pcs | — | DC experiment supply only |
| 14500 battery case | 1 pc | — | Holds 14500 cells |
| Jumper wires M-M | 1 set | — | Arduino and breadboard |
| Jumper wires F-F | 1 set | — | Sensor/module connections |
| Jumper wires M-M / F-F / M-F | 1 set each | — | Breadboard and hardware interface |
| Breadboard | 1 pc | — | Prototyping |
| 150 Ω resistor | 3 pcs | — | Typical 5 V LED series resistor; calculate by LED datasheet |
| 1N5408 diode | 3 pcs | — | Optional flyback diode for inductive DC loads |

> **Assembly note:** For the 3-pole implementation, use 3 FC-51 sensors and 3 MOSFET channels. Extra BOM parts support the single-pole variant, spares, and future experiments.
