#ifndef UI_H
#define UI_H

#include "configParser.h"
#include "image_manager.h"


#define FONT_POWER_DICREASE 1.1
#define FONT_SCALE 25

typedef struct {
  size_t begin;
  size_t end; // vraiment util à voir
} View;

typedef struct {
  Vector2 a,b,c;
} Triangle;

typedef struct {
  Rectangle rec;
  Triangle trl_l;
  Triangle trl_r;
  size_t img_index;
}Form;


// init form
void init_form(Form *f, Config *cfg, size_t index, int *y_position);

void set_index(Form *f,View *v);

// draw part
void draw_text_rect_center(Font font, const char* text, int fontSize, Rectangle rec, Color color);
void draw_form(ImageObjList img_list, Form *f, Config *cfg, bool center);

#endif
