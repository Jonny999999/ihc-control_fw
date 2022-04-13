#include "pulse.hpp"

pulse::pulse (uint32_t msOnDefault_declare, uint32_t msOffDefault_declare){ //constructor
  msOnDefault = msOnDefault_declare;
  msOffDefault = msOffDefault_declare;
}



void pulse::trigger (uint8_t count_in, uint32_t msOn_in, uint32_t msOff_in){ //trigger impulses with custom intervalls/duration
  count += count_in;
  //TODO queue up trigger events
  //currently when beeping is still in progess, but another call of trigger function happens, counts will be added, but msOn, msOff are overwritten immediately
  msOn = msOn_in;
  msOff = msOff_in;
  handle();
}



void pulse::trigger (uint8_t count_in){ //trigger impulses with default intervalls/duration
  count += count_in;
  msOn = msOnDefault;
  msOff = msOffDefault;
  handle();
}



void pulse::handle (){ //handle has to be repeatedly executed (defines state)
  if (state == true){
    //turn off when on long enough
    if ( time_delta(time_get(), timestampOn) >= msOn ){
      state = false;
      timestampOff = time_get();
    }
  }else{
    //turn on if counts left and off long enough
    if ( count >= 1 && time_delta(time_get(), timestampOff) >= msOff ){
      state = true;
      count--;
      timestampOn = time_get();
    }
  }
}

