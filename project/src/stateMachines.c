#include <msp430.h>
#include "stateMachines.h"
#include "led.h"
#include "buzzer.h"

static char state = 0;

char toggle_red() // on, off, on, off... 50% led intensity
{
  switch (state){
  case 0:
    red_on = 1;
    state = 1;
    break;
  case 1:
    red_on = 0;
    state = 0;
    break;
  }
  led_changed = 1;
  led_update();
  return 1; // for state_advance
}

void toggle_red25() // off, off, off, on
{
  switch(state){
  case 0:
    red_on = 0;
    state = 1;
    break;
  case 1:
    red_on = 0;
    state = 2;
    break;
  case 2:
    red_on = 0;
    state = 3;
    break;
  case 3:
    red_on = 1;
    state = 0;
    break;
  }
  led_changed = 1;
  led_update();
}

void toggle_red75() // on, on, on, off
{
  switch(state){
  case 0:
    red_on = 1;
    state = 1;
    break;
  case 1:
    red_on = 1;
    state = 2;
    break;
  case 2:
    red_on = 1;
    state = 3;
    break;
  case 3:
    red_on = 0;
    state = 0;
    break;
  }
  led_changed = 1;
  led_update();
}

char toggle_green()
{
  char changed = 0;
  if(red_on){
    green_on ^=1;
    changed = 1;
  }
  return changed; // for state_advance
}

void dim()
{
  static char state = 0;
  switch(state){
  case 0:               /* no dim */
    red_on = 1;
    led_changed = 1;
    led_update();
    state = 1;
    break;
  case 1:
    toggle_red25();     /* 25% intensity */
    toggle_red25();
    toggle_red25();
    toggle_red25();
    state = 2;
    break;
  case 2:               /* 50% intensity */
    toggle_red();
    toggle_red();
    state = 3;
    break;
  case 3:
    toggle_red75();     /* 75% intensity */
    toggle_red75();
    toggle_red75();
    toggle_red75();
    state = 0;
    break;
  }
}

void song()
{
  int notes[4] = {1000, 2000, 3000, 4000};
  int i = 0;
  while(i < sizeof(notes)){
    buzzer_set_period(notes[i]);
    __delay_cycles(40000000); // delays next note from playing
    i++;
  }
  buzzer_set_period(0);
}

void buzz_advance()              /* called by state_advance to play siren */
{
  static char note = 0;
  switch(note){
  case 0:
    buzzer_set_period(2000000/1000); // cycles = buzzer clock / frequency
    note++;
    break;
  case 1:
    buzzer_set_period(2000000/2000);
    note++;
    break;
  case 2:
    buzzer_set_period(2000000/3000);
    note++;
    break;
  case 3:
    buzzer_set_period(2000000/4000);
    note = 0;
    break;
  default:
    note++;
  }
}

void state_advance()		/* alternate between toggling red & green */
{
  char changed = 0;
  static enum {R=0, G=1} color = G;
  switch (color){
  case R:
    changed = toggle_red();
    color = G;
    buzz_advance();
    break;
  case G:
    changed = toggle_green();
    color = R;
    buzz_advance();
    break;
  }
  led_changed = changed;
  led_update();
}
