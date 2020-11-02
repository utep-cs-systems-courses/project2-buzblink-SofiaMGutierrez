#include <msp430.h>
#include "stateMachines.h"
#include "switches.h"
#include "buzzer.h"
#include "led.h"

void
__interrupt_vec(WDT_VECTOR) WDT(){	/* 250 interrupts/sec */
  static char blink_count = 0;
  if(++blink_count == 2 && S1){
    dim();
    buzzer_set_period(0);
  }
  if(blink_count == 2 && S2){
    song();
  }
  if(blink_count == 125 && S3){
    state_advance();
  }
  if(S4){
    buzzer_set_period(0);
    red_on = 0;
    green_on = 0;
    led_changed = 1;
    led_update();
    blink_count = 0;
  }
}
