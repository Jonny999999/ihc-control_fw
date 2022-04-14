#pragma once

#include <stdint.h>

extern "C" {
#include "time.h" 
}


class pulse {
  public:
    //--- input ---
    pulse (uint32_t msOnDefault_declare, uint32_t msOffDefault_declare);
    uint32_t msOnDefault;
    uint32_t msOffDefault;
    uint32_t msOn = 10;
    uint32_t msOff = 10;

    //--- functions ---
    void trigger (uint8_t count_in, uint32_t msOn_in, uint32_t msOff_in); //trigger impulses with custom intervalls/duration
    void trigger (uint8_t count_in); //trigger impulses with default intervalls/duration

    void handle (); //handle has to be repeatedly executed (defines state)

    //--- output ---
    bool state = false;

  private:
    uint8_t count = 0;
    uint32_t timestampOn = 0;
    uint32_t timestampOff = 0;
};

