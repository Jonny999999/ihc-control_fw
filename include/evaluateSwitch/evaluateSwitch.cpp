#include "evaluateSwitch.hpp"

extern "C" {
    #include "time.h" 
}





evaluatedSwitch::evaluatedSwitch(volatile uint8_t *port_declare, uint8_t pin_declare) { //constructor
  port = port_declare;
  pin = pin_declare;
}



void evaluatedSwitch::handle(){  //Statemachine for debouncing and edge detection
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
        if (time_delta(time_get(), timestampLow) > minOnMs){ //pin in same state long enough
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
        if (time_delta(time_get(), timestampHigh) > minOffMs){ //pin in same state long enough
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


