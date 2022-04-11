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


class inputPin_class {
  public:
  //evaluatedSwitch testButton(&PINB, PB6);
    volatile uint8_t *reg_direction; //DDRx - Data Direction Register (rw)
    volatile uint8_t *reg_data; //PORTx - Data Register (rw)
    volatile uint8_t *reg_inputPin; //PINx - Input Pin Address (ro)
    uint8_t pin;
    bool pullup = true;
    inputPin_class (
        volatile uint8_t *reg_direction_declare,
        volatile uint8_t *reg_data_declare,
        volatile uint8_t *reg_inputPin_declare,
        uint8_t pin_declare
    ){ //constructor
      reg_direction = reg_direction_declare;
      reg_data = reg_data_declare;
      reg_inputPin = reg_inputPin_declare;
      pin = pin_declare;
      init(); //initialize input pin (define as input, enable pullup) TODO case when pullup not wanted?
    }

  private:
    void init(){
      *reg_direction &= ~(1 << pin); //define as input (unset bit)
      *reg_data |= (1 << pin); //enable pullup
    }

};


int main()
{
  time_init();

    //DDRB|=(1<<7); //PB7  define (Buzzer) as output
    //PORTB|=(1<<6); //PB6 enable pull up


  //===== define outputs =====
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


  //===== define inputs =====
  gpio_evaluatedSwitch s4(&DDRB, &PORTB, &PINB, PB2, false, true); //no pullup, inverted
                                                                   //
                                                                   //
  gpio_evaluatedSwitch s2(&DDRB, &PORTB, &PINB, PB4, true, false); //pullup true, inverted true
  gpio_evaluatedSwitch s3(&DDRB, &PORTB, &PINB, PB3, true, false); //pullup true, inverted true




  while(1){


  s2.handle();
  s3.handle();
  s4.handle();



  // if(testButton.state == true){
  //  //if ( !(PINB & (1 << 6)) ){ 
  //  //
  //  led.on();
  // }else{
  //   led.off();
  // }



    //if(testButton.risingEdge && testButton.msReleased > 1000){
    //  PORTB|=(1<<7); //buzzer an
    //    _delay_ms(1000);
    //  PORTB&=~(1<<7); //buzzer aus
    //}
    //if(testButton.risingEdge && testButton.msReleased > 5000){
    //}

    if (s2.state == true){
      k2.on();
      led.on();
    }else{
      k2.off();
      led.off();
    }

    if (s3.state == true){
      k3.on();
    }else{
      k3.off();
    }

    if (s4.state == true){
      buzzer.on();
      k4.on();
    }else{
      buzzer.off();
      k4.off();
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
