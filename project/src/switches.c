#include <msp430.h>
#include "switches.h"
#include "led.h"

char switch_state_down, switch_state_changed; /* effectively boolean */
char S1, S2, S3, S4;

static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
  led_update();
}

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  S1 = (p2val & SW1) ? 0 : 1; /* 0 when SW1 is up */
  S2 = (p2val & SW2) ? 0 : 1;
  S3 = (p2val & SW3) ? 0 : 1;
  S4 = (p2val & SW4) ? 0 : 1;
  if(S1){
    song();
  }
  if(S2){
    //buzzer_set_period(1050);
    //buzzer_set_period(2100);
    buzzer_set_period(3150);
  }
  if(S3){
    //buzzer_set_period(920);
    //buzzer_set_period(1840);
    buzzer_set_period(2760);
  }
  if(S4){
    //buzzer_set_period(780);
    //buzzer_set_period(1560);
    buzzer_set_period(2340);
  }
  switch_state_changed = 1;
  led_update();
}
