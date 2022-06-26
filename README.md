Firmware for a custom pcb with an Atmega8 microcontroller, that controls an IHC-Tractor.

# Links
PCB Project: https://github.com/Jonny999999/ihc-control_pcb (private)  
Connection plan, fuses, connectors: [connection-plan.pdf](connection-plan.pdf)  

# Usage
## First setup
``` bash
git clone git@github.com:Jonny999999/ihc-control_fw.git
cd ihc-control_fw
mkdir build
cd build
cmake ..
```

## Compile
``` bash
cd build
make
```
### Note:
currently the latest version of avr-gcc produces the following error:
```
/ihc-control_fw/include/gpio/a2d.c:11:15: error: array subscript 0 is outside array bounds of 'volatile uint8_t[0]' {aka 'volatile unsigned char[]'} [-Werror=array-bounds]
   11 |         ADMUX &= ~(1<<REFS1);  //Vcc RefU
```
downgrading the package `avr-gcc (12.1.0-1 => 11.3.0-1)` e.g. with `sudo downgrade avr-gcc` is the current workaround


## Upload
Connect avr controller using USBASP V2.0 ISP programmer
``` bash
cd build
sudo make hello_upload
```



# Pin assignment
## Inputs
### terminal TOP left (3x switch to 12V input)

| Pin | Object | Variable | Description |
| --- | --- | --- | --- |
| PB2 | sPB2 | S_LOW_BEAM | key switch low beam |
| PB1 | sPB1 | S_BRAKE | brake switch to 12V |
| PB0 | sPB0 | S_WORK_LIGHT | Toggle switch work light left dashboard |
| GND | - | - | - |

note: Switch parking light directly switches lights since controller is off in this key position (no input necessary)

### terminal bottom left (5x Switch to GND input)
| Pin | Object | Variable | Description |
| --- | --- | --- | --- |
| PB3 | sPB3 | S_HIGH_BEAM | Momentary switch high beam in right dashboard |
| PB4 | sPB4 | S_HORN | Momentary switch for horn in right dashboard |
| PC0 | sPC0 | S_BLINK_LEFT | Toggle switch blink left in right dashboard |
| PC1 | sPC1 | S_BLINK_RIGHT | Toggle switch blink right |
| PC2 | sPC2 | S_WARNING_LIGHTS | Toggle switch warning lights in left dashboard |
| GND | - | - | - |

## Outputs
### Relay outputs (separate terminals right)
| Pin | Object | Variable | Description |
| --- | --- | --- | --- |
| PD5 | k1 | OUT_BLINK_LEFT | blinker rear + connector |
| PB7 | k2 | OUT_BLINK_RIGHT | blinker rear + connector |
| PB6 | k3 | OUT_BLINK_LEFT_FRONT | front blinker only |
| PB4 | k4 | OUT_BLINK_RIGHT_FRONT | front blinker only |
| PD3 | k5 | | free |
| PD2 | k6 | OUT_WORK_LIGHT | rear worklight |
| PD1 | k7 | OUT_HIGH_BEAM | High beam in front |
| PD0 | k8 | OUT_LOW_BEAM | Low beam in front |

### Mosfet outputs (Terminal bottom middle)
| Pin | Object | Variable | Description |
| --- | --- | --- | --- |
| PC4 | mos1 | OUT_HORN | Horn can be switched with mosfet (2 cables and measured max ~4A) |
| PC3 | mos2 | OUT_CONTROL_LAMP | Control lamp in dashboard originally unused (intended "gearbox oil pressure") |



# Custom libraries
## gpio
### gpio_evaluateSwitch.hpp
'gpio_evaluatedSwitch' class for initializing a gpio pin as input and debouncing it, with options to invert or enable pullup.
features:  
- debouncing (minOn minOff ms)
- edge detection (risingEdge, fallingEdge)
- inverting input (e.g. when switching to VCC)
- enable pullup (default enabled/switching to gnd)
- counting time on/off (msPressed, msReleased)
-> see header file for detailed description   

### gpio_output.hpp
'gpio_output' class to initialize and turning on/off a avr gpio pin in a abstracted way  
the constructor initializes/defines the specified pin as output   
with functions .on() off() or setState(bool) the pin can be switched  
-> see header file for detailed description   

### a2d.h
read adc channel  
```c
uint16_t ReadChannel(uint8_t mux);
```


## time
### time.h
Add possibility to get current timestamp and calculate difference between timestamps.  
Used quite much in this project!
```cpp
void time_init(void);  
uint32_t time_get(void);
uint32_t time_delta(uint32_t a, uint32_t b);
```

### clock.hpp
'clock' class to generate a pulsing signal with specified frequency or on/off durations  
e.g. for blinker to have a single/equal pulsing signal that can be used multiple times for left and right without delaying the program with any delay function  
-> see header file for detailed description  

### pulse.hpp
'pulse' class to generate a certain count of pulses with specified durations  
e.g. for beeper without delaying the program with any delay function  
-> see header file for detailed description  


# Notes
