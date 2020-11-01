#include <msp430.h>
#include "stateMachines.h"
#include "led.h"

char toggle_red()
{
  static char state = 0;
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
}

char toggle_red25()
{
  static char state = 0;
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

char toggle_red75()
{
  static char state = 0;
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

char toggle_green()	/* only toggle green if red is on!  */
{
  char changed = 0;
  if (red_on) {
    green_on ^= 1;
    changed = 1;
  }
  return changed;
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
    state = 2;
    break;
  case 2:               /* 50% intensity */
    toggle_red();
    toggle_red();
    state = 3;
    break;
  case 3:
    toggle_red75();     /* 75% intensity */
    state = 0;
    break;
  }
}

void state_advance()		/* alternate between toggling red & green */
{
  char changed = 0;  

  static enum {R=0, G=1} color = G;
  switch (color) {
  case R: changed = toggle_red(); color = G; break;
  case G: changed = toggle_green(); color = R; break;
  }

  led_changed = changed;
  led_update();
}
