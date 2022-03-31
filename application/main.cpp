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

//#include "evaluateSwitch.hpp"



class evaluatedSwitch {
  public:
    //--- input ---
    volatile uint8_t *port;
    uint8_t pin;
    uint32_t minOnMs = 150;
    uint32_t minOffMs = 150;
    evaluatedSwitch(volatile uint8_t *port_declare, uint8_t pin_declare) { //constructor (is run when object of class is created)
      port = port_declare;


      pin = pin_declare;
    }

    //--- output ---         TODO make readonly? (e.g. public section: const int& x = m_x;)
    bool state = false;
    bool risingEdge = false;
    bool fallingEdge = false;
    uint32_t msPressed = 0;
    uint32_t msReleased = 0;

    //--- functions ---
    void handle(){  //Statemachine for debouncing and edge detection
      switch (p_state){
        default:
          p_state = switchState::FALSE;
          break;

        case switchState::FALSE:  //input confirmed high (released)
          fallingEdge = false; //reset edge event
          if ( !(*port & (1 << pin)) ){ //pin low (on)
            p_state = switchState::LOW;
            timestampLow = time_get(); //save timestamp switched from high to low
          } else {
            msReleased = time_delta(time_get(), timestampHigh); //update duration released
          }
          break;

        case switchState::LOW: //input recently switched to low (pressed)
          if ( !(*port & (1 << pin)) ){ //pin low (on)
            if (time_delta(time_get(), timestampLow) > minOnMs){
              p_state = switchState::TRUE;
              state = true;
              risingEdge = true;
              msReleased = time_delta(time_get(), timestampHigh); //calculate duration the button was released
            }
          }else{
            p_state = switchState::FALSE;
          }
          break;

        case switchState::TRUE:  //input confirmed low (pressed)
          risingEdge = false; //reset edge event
          if ( (*port & (1 << pin)) ){ //pin high (off)
            timestampHigh = time_get();
            p_state = switchState::HIGH;
          } else {
            msPressed = time_delta(time_get(), timestampLow); //update duration pressed
          }

          break;

        case switchState::HIGH: //input recently switched to high (released)
          if ( (*port & (1 << pin)) ){ //pin high (off)
            if (time_delta(time_get(), timestampHigh) > minOffMs){
              p_state = switchState::FALSE;
              msPressed = time_delta(time_get(), timestampLow); //calculate duration the button was pressed
              state=false;
              fallingEdge=true;
            }
          }else{
            p_state = switchState::TRUE;
          }
          break;
      }
    }

  private:
    enum class switchState {TRUE, FALSE, LOW, HIGH};
    switchState p_state = switchState::FALSE;
    uint32_t timestampLow = 0;
    uint32_t timestampHigh = 0;
};




int main()
{

  time_init();
  evaluatedSwitch testButton(&PINB, 6);

  while(1){

    DDRB|=(1<<7); //PB7  define (Buzzer) as output
    DDRB|=(1<<3); //PB7  define (Buzzer) as output

    DDRB|=(0<<6); //PB6 define as input
    PORTB|=(1<<6); //PB6 enable pull up

    testButton.handle();

   // if(testButton.state == true){
   //   PORTB|=(1<<7); //buzzer an
   // }else{
   //   PORTB&=~(1<<7); //buzzer aus
   // }

    if(testButton.risingEdge && testButton.msReleased > 1000){
      PORTB|=(1<<7); //buzzer an
        _delay_ms(1000);
      PORTB&=~(1<<7); //buzzer aus
    }
    //if(testButton.risingEdge && testButton.msReleased > 5000){
    //}

  }



  //    while(1)
  //    {
  //      PORTB|=(1<<7); //buzzer an
  //      _delay_ms(100);
  //      PORTB&=~(1<<7); //buzzer aus
  //      _delay_ms(100);
  //    }
}
