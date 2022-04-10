#include "gpio_evaluateSwitch.hpp"

extern "C" {
    #include "time.h" 
}





gpio_evaluatedSwitch::gpio_evaluatedSwitch(
    volatile uint8_t *reg_direction_declare, //DDRx
    volatile uint8_t *reg_data_declare,     //PORTx
    volatile uint8_t *reg_inputPin_declare, //PINx
    uint8_t pin_declare     //PB6 / 6
    ){ //constructor
  reg_direction = reg_direction_declare;
  reg_data = reg_data_declare;
  reg_inputPin = reg_inputPin_declare;
  pin = pin_declare;

  init();
};




void gpio_evaluatedSwitch::init(){
  *reg_direction &= ~(1 << pin); //define as input (unset bit)
  *reg_data |= (1 << pin); //enable pullup
};




void gpio_evaluatedSwitch::handle(){  //Statemachine for debouncing and edge detection
  switch (p_state){
    default:
      p_state = switchState::FALSE;
      break;

    case switchState::FALSE:  //input confirmed high (released)
      fallingEdge = false; //reset edge event
      if ( !(*reg_inputPin & (1 << pin)) ){ //pin low (on)
        p_state = switchState::LOW;
        timestampLow = time_get(); //save timestamp switched from high to low
      } else {
        msReleased = time_delta(time_get(), timestampHigh); //update duration released
      }
      break;

    case switchState::LOW: //input recently switched to low (pressed)
      if ( !(*reg_inputPin & (1 << pin)) ){ //pin low (on)
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
      if ( (*reg_inputPin & (1 << pin)) ){ //pin high (off)
        timestampHigh = time_get();
        p_state = switchState::HIGH;
      } else {
        msPressed = time_delta(time_get(), timestampLow); //update duration pressed
      }

      break;

    case switchState::HIGH: //input recently switched to high (released)
      if ( (*reg_inputPin & (1 << pin)) ){ //pin high (off)
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


