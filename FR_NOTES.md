# Pins
| Pin | Component |
| --- | --------- |
| D10, PB4 | ESC0 vorne links, CCW |
| D11, PB5 | ESC1 vorne rechts, CW |
| D12, PB6 | ESC2 hinten rechts, CCW |
| D13, PB7 | ESC3 hinten links, CW |
| D2, PE4 | Channel 0: Aile |
| D3, PE5 | Channel 1: Elev |
| D18, PD3 | Channel 2: Throt |
| D19, PD2 | Channel 3: Rudd |
| D20 (SDA) | Gyrosensor SDA |
| D21 (SCL) | Gyrosensor SCL |


Mean loop time: 2.6 ms (original: 2.49 ms)

original HighPassFilterCoeff = timeConstant / (dt + timeConstant) = 0.5 / (0.00249 + 0.5) = 0.9995

HighPassFilterCoeff = timeConstant / (dt + timeConstant) = 0.5 / (0.0026 + 0.5) = 0.9995 (bleibt also gleich)
