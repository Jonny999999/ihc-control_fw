#pragma once

#include <stdint.h>


class test {
  public:
    int out;
    int handlefunc();
  private:
    int priv;
};

class evaluatedSwitch {
  public:
    //--- input ---
    volatile uint8_t *port;
    uint8_t pin;
    uint32_t minOnMs = 150;
    uint32_t minOffMs = 150;
    evaluatedSwitch(volatile uint8_t *port_declare, uint8_t pin_declare); //constructor

    //--- output ---         TODO make readonly? (e.g. public section: const int& x = m_x;)
    bool state = false;
    bool risingEdge = false;
    bool fallingEdge = false;
    uint32_t msPressed = 0;
    uint32_t msReleased = 0;

    //--- functions ---
    void handle();  //Statemachine for debouncing and edge detection

  private:
    enum class switchState {TRUE, FALSE, LOW, HIGH};
    switchState p_state = switchState::FALSE;
    uint32_t timestampLow = 0;
    uint32_t timestampHigh = 0;
};


