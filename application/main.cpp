extern "C" {
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <math.h>

#include "time.h"
}

#include "evaluateSwitch.hpp" 





int main()
{

  time_init();
  evaluatedSwitch testButton(&PINB, 6);

  while(1){

    DDRB|=(1<<7); //PB7  define (Buzzer) as output
    DDRB|=(1<<3); //PB7  define (Buzzer) as output

    DDRB|=(0<<6); //PB6 define as input
    PORTB|=(1<<6); //PB6 enable pull up

    testButton.handle();

   // if(testButton.state == true){
   //   PORTB|=(1<<7); //buzzer an
   // }else{
   //   PORTB&=~(1<<7); //buzzer aus
   // }

    if(testButton.risingEdge && testButton.msReleased > 1000){
      PORTB|=(1<<7); //buzzer an
        _delay_ms(1000);
      PORTB&=~(1<<7); //buzzer aus
    }
    //if(testButton.risingEdge && testButton.msReleased > 5000){
    //}

  }



  //    while(1)
  //    {
  //      PORTB|=(1<<7); //buzzer an
  //      _delay_ms(100);
  //      PORTB&=~(1<<7); //buzzer aus
  //      _delay_ms(100);
  //    }
}
