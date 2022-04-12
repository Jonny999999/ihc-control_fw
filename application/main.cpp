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
}

#include "gpio_evaluateSwitch.hpp" 
#include "gpio_output.hpp"
#include "clock.hpp"

//====================================
//===== assign devices to relays =====
//====================================
#define K_LOW_BEAM k1
#define K_HIGH_BEAM k2

#define S_HIGH_BEAM sPB3 //s3



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
  gpio_output mos2(&DDRC, &PORTC, PC4);

  //===== define and configure inputs (create objects) =====
  gpio_evaluatedSwitch sPB2(&DDRB, &PORTB, &PINB, PB2, false, true); //no pullup, inverted (top left 1, switch to 12V)
  gpio_evaluatedSwitch sPB1(&DDRB, &PORTB, &PINB, PB1, false, true); //no pullup, inverted (top left 2, switch to 12V)
  gpio_evaluatedSwitch sPB0(&DDRB, &PORTB, &PINB, PB0, false, true); //no pullup, inverted (top left 3, switch to 12V)
                                                                     //
  gpio_evaluatedSwitch sPB3(&DDRB, &PORTB, &PINB, PB3); //pullup true, inverted false (bottom left 1, switch to GND)
  gpio_evaluatedSwitch sPB4(&DDRB, &PORTB, &PINB, PB4); //pullup true, inverted false (bottom left 2, switch to GND)
  gpio_evaluatedSwitch sPC0(&DDRB, &PORTC, &PINC, PC0); //pullup true, inverted false (bottom left 3, switch to GND)
  gpio_evaluatedSwitch sPC1(&DDRB, &PORTC, &PINC, PC1); //pullup true, inverted false (bottom left 4, switch to GND)
  gpio_evaluatedSwitch sPC2(&DDRB, &PORTC, &PINC, PC2); //pullup true, inverted false (bottom left 5, switch to GND)



  //create clock instance for generating a signal used for blinkers
  //clock blink(200, 100);
  clock blink(2);



  //======================================
  //======= variable declarations ========
  //======================================
    //high beam
    enum class lightState {OFF, ON, ON_TOGGLE};
    lightState highBeamState = lightState::OFF;

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



    switch (highBeamState){
      case lightState::OFF:
        if (S_HIGH_BEAM.risingEdge == true){ //only rising edge, otherwise will instantly turn on again when turining off with same switch
          K_HIGH_BEAM.on();
          highBeamState = lightState::ON;
        }
        break;
      case lightState::ON:
        if (S_HIGH_BEAM.state == true){
          if (S_HIGH_BEAM.msPressed > 750){
            highBeamState = lightState::ON_TOGGLE;
            buzzer.on();
            _delay_ms(100); //TODO dont use delay function anywhere => create beep function (buzzer handle)
            buzzer.off();
          }
        }else{ //switch off
          highBeamState = lightState::OFF;
          K_HIGH_BEAM.off();
        }
        break;
      case lightState::ON_TOGGLE:
        if (S_HIGH_BEAM.risingEdge == true){
          highBeamState = lightState::OFF;
          K_HIGH_BEAM.off();
        }
        break;
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
