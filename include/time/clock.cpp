#include "clock.hpp"




clock::clock (float frequency){ //constructor: freqency given
  setFreq(frequency);
}



clock::clock (uint32_t msOn_declare, uint32_t msOff_declare){ //constructor: timeon, timeoff given
  msOn = msOn_declare;
  msOff = msOff_declare;
  freq = 1 / (msOn + msOff);
}



void clock::setFreq(float frequency){ //change frequency during program
  freq = frequency;
  msOn = 1/frequency * 1000;
  msOff = msOn;
}
  


void clock::setState(bool state_force){ //force state true
  state = state_force;
  if (state == true){
    timestampOn = time_get();
  }else{
    timestampOff = time_get();
  }
}



void clock::handle (void){ //handle function (has to be repeatedly run)
  if (state == true){
    if (time_delta(time_get(), timestampOn) > msOn){
      state = false;
      timestampOff = time_get();
    }
  }else {
    if (time_delta (time_get(), timestampOff) > msOff){
      state = true;
      timestampOn = time_get();
    }
  }
}




