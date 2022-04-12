#pragma once

#include <stdint.h>

extern "C" {
#include "time.h" 
}


class clock {
  public:
    //--- constructors ---
    clock (float frequency); //freqency (Hz) given
    clock (uint32_t msOn_declare, uint32_t msOff_declare); //timeon and timeoff (ms) given

    //--- functions ---
    void handle (void); //handle has to be repeatedly executed in order for the clock to work
    void setState(bool state_force); //immediately force state true or false
    void setFreq(float frequency); //change frequency during program

    //--- input --- (allows changing the interval during program)
    uint32_t msOn;
    uint32_t msOff;

    //--- output ---
    bool state = false;
    float freq = 0; //read only, change using setFreq()

  private:
    uint32_t timestampOn = time_get();
    uint32_t timestampOff = time_get();
};
