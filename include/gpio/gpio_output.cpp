#include "gpio_output.hpp"

extern "C" {
    #include "time.h" 
}


//constructor - define pin variables
gpio_output::gpio_output(
    volatile uint8_t *reg_direction_declare, //DDRx
    volatile uint8_t *reg_data_declare,     //PORTx
    uint8_t pin_declare     //PB6 / 6
    ){ //constructor
  reg_direction = reg_direction_declare;
  reg_data = reg_data_declare;
  pin = pin_declare;

  init();
};


//initialize pin - define as output
void gpio_output::init(){
    *reg_direction|=(1 << pin);  //define pin as output (DDRx)
}


//turn pin on
void gpio_output::on(){
  *reg_data|=(1 << pin);  //turn pin on
  timestampOn = time_get();
}


//turn pin off
void gpio_output::off(){
  *reg_data&=~(1 << pin);  //turn pin off
  timestampOff = time_get();
}
