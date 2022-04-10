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


  gpio_output led(&DDRB, &PORTB, PB7);
  gpio_evaluatedSwitch testButton(&DDRB, &PORTB, &PINB, PB6);


  while(1){


    testButton.handle();



   if(testButton.state == true){
    //if ( !(PINB & (1 << 6)) ){ 
    //
     //PORTB|=(1<<7); //buzzer an
    led.on();
   }else{
     led.off();
     //PORTB&=~(1<<7); //buzzer aus
   }



    //if(testButton.risingEdge && testButton.msReleased > 1000){
    //  PORTB|=(1<<7); //buzzer an
    //    _delay_ms(1000);
    //  PORTB&=~(1<<7); //buzzer aus
    //}
    //if(testButton.risingEdge && testButton.msReleased > 5000){
    //}

        //PORTB|=(1<<7); //buzzer an
        //_delay_ms(100);
        //PORTB&=~(1<<7); //buzzer aus
        //_delay_ms(100);



  }

}
