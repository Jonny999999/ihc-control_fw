#pragma once

#include <stdint.h>

class gpio_output {
  public:
    //--- input ---
    gpio_output (
        volatile uint8_t *reg_direction_declare, //DDRx
        volatile uint8_t *reg_data_declare,     //PORTx
        uint8_t pin_declare     //PB6 / 6
        ); //constructor

    //--- output ---
    bool state;
    //uint32_t msOn; TODO: needs function to calculate time e.g. uint32_t output::getMsOn()
    //uint32_t msOff;
    uint32_t timestampOn;
    uint32_t timestampOff;

    //--- functions ---
    void on();
    void off();

  private:
    //--- pin desciption ---
    volatile uint8_t *reg_direction; //DDRx - Data Direction Register (rw)
    volatile uint8_t *reg_data; //PORTx - Data Register (rw)
    volatile uint8_t *reg_inputPin; //PINx - Input Pin Address (ro)
    uint8_t pin;

    //--- process data ---
    
    //--- functions ---
    void init();
};
