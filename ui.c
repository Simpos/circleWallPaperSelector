#include "ui.h"
#include <math.h>

int c(int r , int i){
  switch(i){
    case 0:
      return r * 0.04;
    case 1:
      return r * 0.06;
    case 2:
      return r * 0.1;
    case 3:
      return r * 0.15;
    case 4:
      return r * 0.2;
    case 5: 
      return r * 0.6;
  }
  return 0;
}

float l(int r, int yi){
  return sqrt(4 * (2 * r * yi - yi * yi));
}

void set_index(Form *f,View *v){
  for(int i = 0; i < 11; ++i){
    f[i].img_index = (i + v->begin) % v->nombre_img;
  }
}


void init_form(Form *f, Config *cfg, size_t index, int *y_position){
  /* 
   * Initialize a form given a radius and an index
   */
  int height,width; 
  if(index <= 5){
    height = c(cfg->RADIUS,index); 
  }else if(index < 10){
    height = c(cfg->RADIUS, 5 - index % 5);
  }else{
    height = c(cfg->RADIUS, 0);
  }
  width =  l(cfg->RADIUS,*y_position + height / 2);
  f->rec = (Rectangle){
    .x = cfg->RADIUS + cfg->OFFSET - width / 2,
    .y = *y_position,
    .width = width,
    .height = height
  };

  float arrow_paramater = index != 5 ? 1.15 : 1.22;

  f->trl_l = (Triangle){
      .a = (Vector2){.x = cfg->RADIUS + cfg->OFFSET - width / 2, .y = *y_position}, 
      .b = (Vector2){.x = cfg->RADIUS + cfg->OFFSET - width/2, .y = *y_position + height}, 
      .c = (Vector2){.x = cfg->RADIUS + cfg->OFFSET - width / 2 - pow(width,arrow_paramater) * 0.1, .y = *y_position + height / 2 }};
  f->trl_r = (Triangle){
      .a = (Vector2){.x = cfg->RADIUS + cfg->OFFSET + width / 2, .y = *y_position}, 
      .b = (Vector2){.x = cfg->RADIUS + cfg->OFFSET + width/2, .y = *y_position + height}, 
      .c = (Vector2){.x = cfg->RADIUS + cfg->OFFSET + width / 2 + pow(width,arrow_paramater)* 0.1, .y = *y_position + height / 2 }};
 

  *(y_position) += height + cfg->P;
  f->img_index = index;
}

void draw_text_rect_center(Font font, const char* text, int fontSize, Rectangle rec, Color color){
  /*
   * Draw a text centered in a rectangle
   */
  Vector2 text_measure = MeasureTextEx(font,text,fontSize,1);  
  DrawText(text,rec.x + rec.width /  2 - text_measure.x / 2,
                rec.y + rec.height / 2 - text_measure.y / 2,fontSize,color);
}


static int get_font_size(Form *f){
  return pow(f->rec.width,FONT_POWER_DICREASE) / FONT_SCALE;
} 

void draw_form(ImageObjList img_list, Form *f, Config *cfg, bool center){
  Color color = center ? GetColor(cfg->BG_HOVER_COLOR) : GetColor(cfg->BG_COLOR);
  Color color_fg = center ? GetColor(cfg->FG_HOVER_COLOR) : GetColor(cfg->FG_COLOR);

  //DrawRectangleRec(f->rec, color);
  //DrawTriangle(f->trl_l.a,f->trl_l.c,f->trl_l.b,color);
  //DrawTriangle(f->trl_r.a,f->trl_r.b,f->trl_r.c,color);
  if(cfg->ENABLE_TEXTURE)DrawTextureV(img_list.list[f->img_index].texture,(Vector2){.x = f->rec.x, .y = f->rec.y},WHITE);
  if(cfg->ENABLE_FILENAME)draw_text_rect_center(GetFontDefault(),img_list.list[f->img_index].file_path,get_font_size(f),f->rec,color_fg);
}

