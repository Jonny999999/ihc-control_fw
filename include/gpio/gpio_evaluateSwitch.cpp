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

    //--- get pin state ---
    if ( (*reg_inputPin & (1 << pin)) ){ //pin high
        inputState = false;
    } else { //pin low
        inputState = true;
    }

    //--- invert state ---
    //not inverted: switch switches to GND when active
    //inverted: switch switched to VCC when active
    if (inverted == true){
        inputState = !inputState;
    }



    //=====================================================
    //=================== Statemachine ====================
    //=====================================================
    switch (p_state){
      default:
        p_state = switchState::FALSE;
        break;

      case switchState::FALSE:  //input confirmed released/false
        fallingEdge = false; //reset edge event
        if ( inputState == true ){ //pin on
          p_state = switchState::ACTIVE;
          timestampLow = time_get(); //save timestamp switched from high to low
        } else {
          msReleased = time_delta(time_get(), timestampHigh); //update duration released
        }
        break;

      case switchState::ACTIVE: //input recently switched to true
        if ( inputState == true ){ //pin still on
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

      case switchState::TRUE:  //input confirmed pressed/true
        risingEdge = false; //reset edge event
        if ( inputState == false ){ //pin off
          timestampHigh = time_get();
          p_state = switchState::INACTIVE;
        } else {
          msPressed = time_delta(time_get(), timestampLow); //update duration pressed
        }

        break;

      case switchState::INACTIVE: //input recently switched to false
        if ( inputState == false ){ //pin still off
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


