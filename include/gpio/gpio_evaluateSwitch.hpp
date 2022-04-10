#pragma once

#include <stdint.h>

class gpio_evaluatedSwitch {
  public:
    //--- input ---
    uint32_t minOnMs = 150;
    uint32_t minOffMs = 150;
    gpio_evaluatedSwitch(
        volatile uint8_t *reg_direction_declare, //DDRx
        volatile uint8_t *reg_data_declare,     //PORTx
        volatile uint8_t *reg_inputPin_declare, //PINx
        uint8_t pin_declare     //PB6 / 6
        ); //constructor

    //--- output ---         TODO make readonly? (e.g. public section: const int& x = m_x;)
    bool state = false;
    bool risingEdge = false;
    bool fallingEdge = false;
    uint32_t msPressed = 0;
    uint32_t msReleased = 0;

    //--- functions ---
    void handle();  //Statemachine for debouncing and edge detection

  private:
    volatile uint8_t *reg_direction; //DDRx - Data Direction Register (rw)
    volatile uint8_t *reg_data; //PORTx - Data Register (rw)
    volatile uint8_t *reg_inputPin; //PINx - Input Pin Address (ro)
    uint8_t pin;

    enum class switchState {TRUE, FALSE, LOW, HIGH};
    switchState p_state = switchState::FALSE;
    uint32_t timestampLow = 0;
    uint32_t timestampHigh = 0;
    void init();
};


