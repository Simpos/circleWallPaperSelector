#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <stddef.h>
#include  "raylib.h"

typedef struct {
  bool up;
  bool down;
  float scroll_rate;
  float scroll_delay;
  float next_scroll_time;
  size_t *current_index;
  int number_img;
}Scroller;

void reset_scroller(Scroller *scr);
void scroll(Scroller *scr, bool *addr);
void move(Scroller *scr);
void press(Scroller *scr,bool *addr);

#endif
