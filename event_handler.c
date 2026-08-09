#include "event_handler.h"

void scroll(Scroller *scr, bool *addr){
  /*
   *
   */
  if(GetTime() >= scr->next_scroll_time){
    (*addr) = true;
    scr->next_scroll_time = GetTime() + scr->scroll_rate;
  }
}


void move(Scroller *scr){
  if(scr->up){
    *(scr->current_index) = *(scr->current_index) - 1;
  }
  if(scr->down){
    *(scr->current_index) = *(scr->current_index) + 1;
  }
  *(scr->current_index) = *(scr->current_index) % scr->number_img;
}

void reset_scroller(Scroller *scr){
  scr->up = false;
  scr->down = false;
}

void press(Scroller *scr,bool *addr){
  scr->next_scroll_time = GetTime() + scr->scroll_delay;
  *(addr) = true;
}
