#pragma once

#include <stdint.h>

//constructor examples:
  //switch to gnd and us internal pullup:
    //gpio_evaluatedSwitch s3(&DDRB, &PORTB, &PINB, PB3);
  //switch to gnd dont use internal pullup:
    //gpio_evaluatedSwitch s3(&DDRB, &PORTB, &PINB, PB3, false);
  //switch to VCC (inverted) and dont use internal pullup:
    //gpio_evaluatedSwitch s3(&DDRB, &PORTB, &PINB, PB3, false, true);


class gpio_evaluatedSwitch {
  public:
    //--- input ---
    uint32_t minOnMs = 50;
    uint32_t minOffMs = 50;
    gpio_evaluatedSwitch( //constructor minimal (default parameters pullup=true, inverted=false)
        volatile uint8_t *reg_direction_declare, //DDRx
        volatile uint8_t *reg_data_declare,     //PORTx
        volatile uint8_t *reg_inputPin_declare, //PINx
        uint8_t pin_declare     //PB6 / 6
        );
    gpio_evaluatedSwitch( //constructor with optional parameters
        volatile uint8_t *reg_direction_declare, //DDRx
        volatile uint8_t *reg_data_declare,     //PORTx
        volatile uint8_t *reg_inputPin_declare, //PINx
        uint8_t pin_declare,     //PB6 / 6
        bool pullup_declare,
        bool inverted_declare=true
        );

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
    bool pullup;
    bool inverted;

    enum class switchState {TRUE, FALSE, LOW, HIGH};
    switchState p_state = switchState::FALSE;
    uint32_t timestampLow = 0;
    uint32_t timestampHigh = 0;
    void init();

    bool state_inverted = false;
    bool risingEdge_tmp = risingEdge;
    uint32_t msPressed_tmp = msPressed;
};


