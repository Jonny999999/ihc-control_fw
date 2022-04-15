Firmware for ihc-control pcb
Using Atmega8 AVR micocontroller.

# Usage
## Compile


## Upload

# Pin assignment

## Inputs
### terminal top left (3x switch to 12V input)

| Pin | Object | Variable | Description |
| --- | --- | --- | --- |
| PB2 | sPB2 | S_LOW_BEAM | key switch low beam |
| PB1 | sPB1 | S_BRAKE | brake switch to 12V |
| PB0 | sPB0 | ??? | free for worklight? |
| GND | - | - | - |

note: Switch parking light directly switches lights since controller is off in this key position (no input necessary)

### terminal bottom left (5x Switch to GND input)
| Pin | Object | Variable | Description |
| --- | --- | --- | --- |
| PB3 | sPB3 | S_HIGH_BEAM | Momentary switch high beam in dashboard |
| PB4 | sPB4 | S_HORN | Momentary switch for horn in dashboard |
| PC0 | sPC0 | S_BLINK_LEFT | Toggle switch blink left |
| PC1 | sPC1 | S_BLINK_RIGHT | Toggle switch blink right |
| PC2 | sPC2 | S_WARNING_LIGHTS | Toggle switch warning lights in dashboard |
| GND | - | - | - |

## Outputs
### Relay outputs (separate terminals right)
| Pin | Object | Variable | Description |
| PD5 | k1 | OUT_BLINK_LEFT | toggle switch blinker left in dashboard |
| PB7 | k2 | OUT_BLINK_RIGHT | toggle switch blinker right in dashboard |
| PB6 | k3 | OUT_PARKING_LIGHT | ??? necessary? can be switched directly |
| PB4 | k4 | | |
| PD3 | k5 | | |
| PD2 | k6 | ??? | free for worklight? |
| PD1 | k7 | OUT_HIGH_BEAM | High beam in front |
| PD0 | k8 | OUT_LOW_BEAM | Low beam in front |

### Mosfet outputs (Terminal bottom middle)
| Pin | Object | Variable | Description |
| PC4 | mos1 | OUT_HORN | Horn can be switched with mosfet (2 cables max ~4A) |
| PC3 | mos2 | | |





# Notes
