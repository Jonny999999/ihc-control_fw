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
#define LOW_BEAM k1
#define HIGH_BEAM k2




int main()
{
  time_init();

  //DDRB|=(1<<7); //PB7  define (Buzzer) as output
  //PORTB|=(1<<6); //PB6 enable pull up


  //===== define and configure outputs =====
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


  //===== define and configure inputs =====
  gpio_evaluatedSwitch s4(&DDRB, &PORTB, &PINB, PB2, false, true); //no pullup, inverted
                                                                   //
                                                                   //
  gpio_evaluatedSwitch s2(&DDRB, &PORTB, &PINB, PB4, true, false); //pullup true, inverted true
  gpio_evaluatedSwitch s3(&DDRB, &PORTB, &PINB, PB3, true, false); //pullup true, inverted true




  //create clock instance for generating a signal for blinkers
  //clock blink(200, 100);
  clock blink(2);


  while(1){

    //run handle function for each switch TODO: loop over / run all switch instances automatically?
    s2.handle();
    s3.handle();
    s4.handle();

    //run handle function of clock object
    blink.handle();




    //testing clock class with button s2 (blink led + relay)
    if (s2.state == true){
      if (s2.risingEdge == true){
        blink.setState(true); //force reset to true (alwas start with led on)
      }
      if (blink.state == true){
        led.on();
        k2.on();
      }else{
        led.off();
        k2.off();
      }
    }else{
      led.off();
      k2.off();
    }


    //testing button s3
    if (s3.state == true){
      k3.on();
      if(s3.risingEdge){ //increase blink frequency
        blink.setFreq(blink.freq + 10);
      }
    }else{
      k3.off();
    }

    if (s3.risingEdge && s3.msReleased > 5000){
      buzzer.on();
      _delay_ms(100);
      buzzer.off();
    }

    if (s3.fallingEdge && s3.msPressed > 2000){
      buzzer.on();
      _delay_ms(500);
      buzzer.off();
    }


    //testing button s4
    if (s4.state == true){
      if (s4.risingEdge){ //decrease blink frequency
        blink.setFreq(blink.freq - 10);
      }
      led.on();
      k4.on();
    }else{
      k4.off();
      led.off();
    }

    if (s4.risingEdge && s4.msReleased > 5000){
      buzzer.on();
      _delay_ms(100);
      buzzer.off();
    }

    if (s4.fallingEdge && s4.msPressed > 2000){
      buzzer.on();
      _delay_ms(500);
      buzzer.off();
    }

    //    //blink
    //    led.on();
    //    //buzzer.on();
    //    _delay_ms(100);
    //    led.off();
    //    //buzzer.off();
    //    _delay_ms(100);
    //


  }

}
