extern "C" {
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <math.h>

#include "time.h"
#include "a2d.h"
}

#include "gpio_evaluateSwitch.hpp" 
#include "gpio_output.hpp"
#include "clock.hpp"
#include "pulse.hpp"


//============================
//========== config ==========
//============================
//#define SILENT //disable buzzer
#define VOLTAGE_CHECK_INTERVAL 15000 //low voltage check (ms)
#define VOLTAGE_CHECK_INITIAL 1200 //time first check


//=====================================
//===== assign devices to outputs =====
//=====================================
//--- 8x relay outputs ---
#define OUT_BLINK_LEFT k1
#define OUT_BLINK_RIGHT k2
#define OUT_BLINK_LEFT_FRONT k3
#define OUT_BLINK_RIGHT_FRONT k4
//#define OUT_ k5
#define OUT_WORK_LIGHT k6
#define OUT_HIGH_BEAM k7
#define OUT_LOW_BEAM k8

//--- 2x mosfet outputs ---
#define OUT_HORN mos1
#define OUT_CONTROL_LAMP mos2


//====================================
//===== assign devices to inputs =====
//====================================
//--- 2x inputs (switches to 12V) ---
#define S_LOW_BEAM sPB2 //s3
#define S_BRAKE sPB1
#define S_WORK_LIGHT sPB0

//--- 4x inpts (switches to GND) ---
#define S_HIGH_BEAM sPB3
#define S_HORN sPB4
#define S_BLINK_LEFT sPC0
#define S_BLINK_RIGHT sPC1
#define S_WARNING_LIGHTS sPC2




int main()
{
  //initialize time library
  time_init();

  //========================================
  //========= configure GPIO pins ==========
  //========================================
  //DDRB|=(1<<7); //PB7  define (Buzzer) as output
  //PORTB|=(1<<6); //PB6 enable pull up

  //===== define and configure outputs (create objects) =====
  gpio_output led(&DDRD, &PORTD, PD7);
  gpio_output buzzer(&DDRD, &PORTD, PB6);

  gpio_output k1(&DDRD, &PORTD, PD5);
  gpio_output k2(&DDRB, &PORTB, PB7);
  gpio_output k3(&DDRB, &PORTB, PB6);
  gpio_output k4(&DDRD, &PORTD, PD4);
  gpio_output k5(&DDRD, &PORTD, PD3);
  gpio_output k6(&DDRD, &PORTD, PD2);
  gpio_output k7(&DDRD, &PORTD, PD1);
  gpio_output k8(&DDRD, &PORTD, PD0);

  gpio_output mos1(&DDRC, &PORTC, PC4);
  gpio_output mos2(&DDRC, &PORTC, PC3);

  //===== define and configure inputs (create objects) =====
  gpio_evaluatedSwitch sPB2(&DDRB, &PORTB, &PINB, PB2, false, true); //no pullup, inverted (top left 1, switch to 12V)
  gpio_evaluatedSwitch sPB1(&DDRB, &PORTB, &PINB, PB1, false, true); //no pullup, inverted (top left 2, switch to 12V)
  gpio_evaluatedSwitch sPB0(&DDRB, &PORTB, &PINB, PB0, false, true); //no pullup, inverted (top left 3, switch to 12V)
                                                                     //
  gpio_evaluatedSwitch sPB3(&DDRB, &PORTB, &PINB, PB3); //pullup true, inverted false (bottom left 1, switch to GND)
  gpio_evaluatedSwitch sPB4(&DDRB, &PORTB, &PINB, PB4); //pullup true, inverted false (bottom left 2, switch to GND)
  gpio_evaluatedSwitch sPC0(&DDRC, &PORTC, &PINC, PC0); //pullup true, inverted false (bottom left 3, switch to GND)
  gpio_evaluatedSwitch sPC1(&DDRC, &PORTC, &PINC, PC1); //pullup true, inverted false (bottom left 4, switch to GND)
  gpio_evaluatedSwitch sPC2(&DDRC, &PORTC, &PINC, PC2); //pullup true, inverted false (bottom left 5, switch to GND)



  //=========================================
  //========= Create other objects ==========
  //=========================================
  //create clock instance for generating a signal used for blinkers
  //clock blink(200, 100);
  clock blink(2);

  //create pulse instance for generating pulses for buzzer without using _delay_ms() thus pausing the entire programm.
  pulse beep(200, 100); //default msOn, msOff




  //======================================
  //======= variable declarations ========
  //======================================
    //high beam
    enum class lightState {OFF, ON, ON_TOGGLE};
    lightState highBeamState = lightState::OFF;

    //blink
    uint32_t timestampBlinkStart = 0;

    //input voltage
    uint32_t slowLoopLastRun = 0; //first voltage check 1.5s after startup)
    uint16_t adcValue = 0;
    float inputVoltage = 0;
	uint32_t nextCheckInterval = VOLTAGE_CHECK_INITIAL;


    //--------------------------------
    //-------- Startup tasks ---------
    //--------------------------------
    //beep 3 times
    beep.trigger(3, 80, 50);



  //======================================
  //========= Start of main loop =========
  //======================================
    while(1){

    //run handle function for each switch TODO: loop over / run all switch instances automatically?
    sPB2.handle();
    sPB1.handle();
    sPB0.handle();
    sPB3.handle();
    sPB4.handle();
    sPC0.handle();
    sPC1.handle();
    sPC2.handle();

    //run handle function of clock object
    blink.handle();

    //run handle function of pulse object 
    beep.handle();



    //------------------------------------
    //------ Low voltage detection -------
    //------------------------------------
    //slow loop
      if (time_delta(time_get(), slowLoopLastRun) > nextCheckInterval){
        slowLoopLastRun = time_get();
		nextCheckInterval = VOLTAGE_CHECK_INTERVAL;
		//TODO: vary check interval in some way depending on last voltage?

        //measure input voltage
        adcValue = ReadChannel(5); //read ADC channel 5 (PC5)
        inputVoltage = (float)adcValue / 1024 * 5 * 3.311; //scale adc value to adc voltage, scale adc voltage to input voltate (voltage divider)
        //beep.trigger((int)inputVoltage, 50, 100); //debug: output beep for each volt
        //thresholds, alerts
		//TODO variable interval depending on voltage?
        if (inputVoltage < 10.6){ //critical
          beep.trigger(10, 200, 100);
        }else if(inputVoltage < 11){ //0%
          beep.trigger(4, 150, 200);
        }else if(inputVoltage < 11.8){ //<25%
          beep.trigger(1, 80, 100);
        }else{
        }
      }



    //-------------------------------------
    //------- Buzzer, Control-lamp --------
    //-------------------------------------
    //TODO Add way to disable buzzer (e.g. when switch pressed during startup)
    //TODO Use control-lamp for different things than buzzer? with a separate pulse object
    //apply state defined by pulse object beep to buzzer and control-lamp output
#ifndef SILENT
    buzzer.setState(beep.state);
#endif
    OUT_CONTROL_LAMP.setState(beep.state);
    //buzzer/control lamp can now be used with e.g.
      //beep.trigger(3); //beep 3 times, default times (see constructor)
      //beep.trigger(3, 1000, 200); //beep 3 times, 1s on 200ms off



    //-----------------------------
    //-------- Work light ---------
    //-----------------------------
    //set output to same state as input switch
    OUT_WORK_LIGHT.setState(S_WORK_LIGHT.state);



    //-----------------------------
    //--------- Low Beam ----------
    //-----------------------------
    //set output pin to the result of the following condition
    OUT_LOW_BEAM.setState(S_LOW_BEAM.state == true && OUT_HIGH_BEAM.state == false); //turn low beam off if high beam is on (otherwise lamp would overheat)
    //Note: parking light is switched by key switch directly (only controlling 2x front lights)



    //------------------------------
    //--------- High Beam ----------
    //------------------------------
    switch (highBeamState){
      case lightState::OFF:
        if (S_HIGH_BEAM.risingEdge == true){ //only rising edge, otherwise will instantly turn on again when turining off with same switch
          OUT_HIGH_BEAM.on();
          highBeamState = lightState::ON;
        }
        break;
      case lightState::ON:
        if (S_HIGH_BEAM.state == true){
          if (S_HIGH_BEAM.msPressed > 600 && S_LOW_BEAM.state == true){ //only able to toggle on if low beam is on
            highBeamState = lightState::ON_TOGGLE;
            beep.trigger(2, 500, 100);
          }
        }else{ //switch off
          highBeamState = lightState::OFF;
          OUT_HIGH_BEAM.off();
        }
        break;
      case lightState::ON_TOGGLE:
        if (S_HIGH_BEAM.risingEdge == true || S_LOW_BEAM.state == false){ //also switch off when low beam is switched off
          highBeamState = lightState::OFF;
          OUT_HIGH_BEAM.off();
        }
        break;
    }



    //-----------------------------
    //----------- Horn ------------
    //-----------------------------
    //set output to same state as input switch
    OUT_HORN.setState(S_HORN.state);



    //----------------------------------
    //-------- Blinker general ---------
    //----------------------------------
    //reset blinking clock
    if ( S_BLINK_LEFT.risingEdge || S_BLINK_RIGHT.risingEdge || S_WARNING_LIGHTS.risingEdge ){
      blink.setState(true); //force clock to true state (for blinkers to be immediately on after switching on, then blinking)
    }
    //reset timestamp when blinking starts (for actions after certain time blinking)
    if ( S_BLINK_LEFT.risingEdge || S_BLINK_RIGHT.risingEdge){
      timestampBlinkStart = time_get();
    }

    //start beeping when blinking a certain time already (not when using warning lights)
    if ( (S_BLINK_LEFT.state || S_BLINK_RIGHT.state) 
            && !S_WARNING_LIGHTS.state 
            && time_delta(time_get(), timestampBlinkStart) > 5000 
            && blink.risingEdge
       ){
        beep.trigger(2, 100, 80);
    }



    //----------------------------------------------
    //--------------- Blinker left -----------------
    //--- Brake light, Warning light, Blink left ---
    //----------------------------------------------
    if (S_BRAKE.state && !S_BLINK_LEFT.state && !S_WARNING_LIGHTS.state){ //condition for brake light
      OUT_BLINK_LEFT.on(); //only use rear blinkers as brake lights
      OUT_BLINK_LEFT_FRONT.off();

    }else if ( (S_BLINK_LEFT.state || S_WARNING_LIGHTS.state)  && blink.state ){ //condition for blinking right and warning light
      OUT_BLINK_LEFT.on();
      OUT_BLINK_LEFT_FRONT.on(); //use rear and front blinkers

    }else{
      OUT_BLINK_LEFT.off();
      OUT_BLINK_LEFT_FRONT.off();
    }



    //----------------------------------------------
    //--------------- Blinker right ----------------
    //--- Brake light, Warning light, Blink left ---
    //----------------------------------------------
    if (S_BRAKE.state && !S_BLINK_RIGHT.state && !S_WARNING_LIGHTS.state) {//condition for brake light
      OUT_BLINK_RIGHT.on(); //only use rear blinkers as brake lights
      OUT_BLINK_RIGHT_FRONT.off();

    }else if ( (S_BLINK_RIGHT.state || S_WARNING_LIGHTS.state)  && blink.state ){ //condition for blinking right and warning light
      OUT_BLINK_RIGHT.on();
      OUT_BLINK_RIGHT_FRONT.on(); //use rear and front blinkers

    }else{
      OUT_BLINK_RIGHT.off();
      OUT_BLINK_RIGHT_FRONT.off();
    }



    //=======================================
    //============== TESTING ================
    //=======================================

//    //testing clock class with button sPB4 (blink led + relay)
//    if (sPB4.state == true){
//      if (sPB4.risingEdge == true){
//        blink.setState(true); //force reset to true (alwas start with led on)
//      }
//      if (blink.state == true){
//        led.on();
//        k2.on();
//      }else{
//        led.off();
//        k2.off();
//      }
//    }else{
//      led.off();
//      k2.off();
//    }
//
//
//    //testing button sPB3
//    if (sPB3.state == true){
//      k3.on();
//      if(sPB3.risingEdge){ //increase blink frequency
//        blink.setFreq(blink.freq + 10);
//      }
//    }else{
//      k3.off();
//    }
//
//    if (sPB3.risingEdge && sPB3.msReleased > 5000){
//      buzzer.on();
//      _delay_ms(100);
//      buzzer.off();
//    }
//
//    if (sPB3.fallingEdge && sPB3.msPressed > 2000){
//      buzzer.on();
//      _delay_ms(500);
//      buzzer.off();
//    }
//
//
//    //testing button sPB2
//    if (sPB2.state == true){
//      if (sPB2.risingEdge){ //decrease blink frequency
//        blink.setFreq(blink.freq - 10);
//      }
//      led.on();
//      k4.on();
//    }else{
//      k4.off();
//      led.off();
//    }
//
//    if (sPB2.risingEdge && sPB2.msReleased > 5000){
//      buzzer.on();
//      _delay_ms(100);
//      buzzer.off();
//    }
//
//    if (sPB2.fallingEdge && sPB2.msPressed > 2000){
//      buzzer.on();
//      _delay_ms(500);
//      buzzer.off();
//    }
//
  }

}
