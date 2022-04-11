#include "gpio_evaluateSwitch.hpp"

extern "C" {
#include "time.h" 
}



gpio_evaluatedSwitch::gpio_evaluatedSwitch( //minimal (use default values)
    volatile uint8_t *reg_direction_declare, //DDRx
    volatile uint8_t *reg_data_declare,     //PORTx
    volatile uint8_t *reg_inputPin_declare, //PINx
    uint8_t pin_declare     //PB6 / 6
    ){
  reg_direction = reg_direction_declare;
  reg_data = reg_data_declare;
  reg_inputPin = reg_inputPin_declare;
  pin = pin_declare;
  pullup = true;
  inverted = false;

  init();
};



gpio_evaluatedSwitch::gpio_evaluatedSwitch( //optional parameters given
    volatile uint8_t *reg_direction_declare, //DDRx
    volatile uint8_t *reg_data_declare,     //PORTx
    volatile uint8_t *reg_inputPin_declare, //PINx
    uint8_t pin_declare,     //PB6 / 6
    bool pullup_declare,
    bool inverted_declare //default false (see hpp file)
    ){
  reg_direction = reg_direction_declare;
  reg_data = reg_data_declare;
  reg_inputPin = reg_inputPin_declare;
  pin = pin_declare;
  pullup = pullup_declare;
  inverted = inverted_declare;

  init();
};



void gpio_evaluatedSwitch::init(){
  *reg_direction &= ~(1 << pin); //define as input (unset bit)
  if (pullup == true){ //enable pullup if desired (default)
    *reg_data |= (1 << pin); //enable pullup
  }
};



void gpio_evaluatedSwitch::handle(){  //Statemachine for debouncing and edge detection
  if (inverted == true){
    //=========================================================
    //=========== Statemachine for inverted switch ============
    //=================== (switch to VCC) =====================
    //=========================================================
    switch (p_state){
      default:
        p_state = switchState::FALSE;
        break;

      case switchState::FALSE:  //input confirmed high (released)
        fallingEdge = false; //reset edge event
        if ( (*reg_inputPin & (1 << pin)) ){ //pin high (on)
          p_state = switchState::HIGH;
          timestampHigh = time_get(); //save timestamp switched from low to high
        } else {
          msReleased = time_delta(time_get(), timestampLow); //update duration released
        }
        break;

      case switchState::HIGH: //input recently switched to high (pressed)
        if ( (*reg_inputPin & (1 << pin)) ){ //pin still high (on)
          if (time_delta(time_get(), timestampHigh) > minOnMs){ //pin in same state long enough
            p_state = switchState::TRUE;
            state = true;
            risingEdge = true;
            msReleased = time_delta(timestampHigh, timestampLow); //calculate duration the button was released 
          }
        }else{
          p_state = switchState::FALSE;
        }
        break;

      case switchState::TRUE:  //input confirmed high (pressed)
        risingEdge = false; //reset edge event
        if ( !(*reg_inputPin & (1 << pin)) ){ //pin low (off)
          timestampLow = time_get();
          p_state = switchState::LOW;
        } else {
          msPressed = time_delta(time_get(), timestampHigh); //update duration pressed
        }

        break;

      case switchState::LOW: //input recently switched to low (released)
        if ( !(*reg_inputPin & (1 << pin)) ){ //pin still low (off)
          if (time_delta(time_get(), timestampLow) > minOffMs){ //pin in same state long enough
            p_state = switchState::FALSE;
            msPressed = time_delta(timestampLow, timestampHigh); //calculate duration the button was pressed
            state=false;
            fallingEdge=true;
          }
        }else{
          p_state = switchState::TRUE;
        }
        break;
    }

  }else{
    //=========================================================
    //========= Statemachine for not inverted switch ==========
    //=================== (switch to GND) =====================
    //=========================================================
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
        if ( !(*reg_inputPin & (1 << pin)) ){ //pin still low (on)
          if (time_delta(time_get(), timestampLow) > minOnMs){ //pin in same state long enough
            p_state = switchState::TRUE;
            state = true;
            risingEdge = true;
            msReleased = time_delta(timestampLow, timestampHigh); //calculate duration the button was released
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
        if ( (*reg_inputPin & (1 << pin)) ){ //pin still high (off)
          if (time_delta(time_get(), timestampHigh) > minOffMs){ //pin in same state long enough
            p_state = switchState::FALSE;
            msPressed = time_delta(timestampHigh, timestampLow); //calculate duration the button was pressed
            state=false;
            fallingEdge=true;
          }
        }else{
          p_state = switchState::TRUE;
        }
        break;
    }
  }

}


