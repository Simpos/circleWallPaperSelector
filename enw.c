#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "image_manager.h"
#include "ui.h"
#include "event_handler.h"

#include "da_header.h"

#define CONFIG_PARSER_IMPLEMENTATION
#include "configParser.h"

int init_config(Config *cfg,const char* file_path){
  init_default_config(cfg);
  ConfigMapping *cfg_map = NULL;
  da_push(cfg_map,((ConfigMapping){"window-title",  TYPE_STRING, &(cfg->W_TITLE)}));
  da_push(cfg_map,((ConfigMapping){"folder-path",  TYPE_STRING, &(cfg->IMG_FOLDER)}));

  da_push(cfg_map,((ConfigMapping){"scroll-delay",  TYPE_FLOAT, &(cfg->SCROLL_DELAY)}));
  da_push(cfg_map,((ConfigMapping){"scroll-rate",  TYPE_FLOAT, &(cfg->SCROLL_RATE)}));

  da_push(cfg_map,((ConfigMapping){"background-color",  TYPE_LONG_INT, &(cfg->BG_COLOR)}));
  da_push(cfg_map,((ConfigMapping){"foreground-color",  TYPE_LONG_INT, &(cfg->FG_COLOR)}));
  da_push(cfg_map,((ConfigMapping){"background-hover-color",  TYPE_LONG_INT, &(cfg->BG_HOVER_COLOR)}));


  da_push(cfg_map,((ConfigMapping){"view-count",  TYPE_INT, &(cfg->VIEW_COUNT)}));
  da_push(cfg_map,((ConfigMapping){"font-size",  TYPE_INT, &(cfg->FONT_SIZE)}));
  da_push(cfg_map,((ConfigMapping){"text-x-offset",  TYPE_INT, &(cfg->TEXT_X_OFFSET)}));
  da_push(cfg_map,((ConfigMapping){"max-number-img",  TYPE_INT, &(cfg->MAX_NUMBER_IMG)}));
  da_push(cfg_map,((ConfigMapping){"enable-flag",  TYPE_INT, &(cfg->ENABLE_FLAG)}));
  da_push(cfg_map,((ConfigMapping){"offset",  TYPE_INT, &(cfg->OFFSET)}));
  da_push(cfg_map,((ConfigMapping){"radius",  TYPE_INT, &(cfg->RADIUS)}));
  da_push(cfg_map,((ConfigMapping){"silent",  TYPE_INT, &(cfg->SILENT)}));
  da_push(cfg_map,((ConfigMapping){"log-info",  TYPE_INT, &(cfg->LOG_INFO)}));

  da_push(cfg_map,((ConfigMapping){NULL, 0, NULL}));

  if(cfg_map == NULL){
    fprintf(stderr, "Memory allocation error during init_config\n");
    return 1;
  }

  if(!read_file("/home/sim/code/circleRay/default-config",cfg,cfg_map)){
    fprintf(stderr, "Couldn't load the config file at path %s, using default config\n",file_path);
    return 1;
  }

  if(!read_file(file_path,cfg,cfg_map)){
    fprintf(stderr, "Couldn't load the config file at path %s, using default config\n",file_path);
    return 1;
  }

  da_free(cfg_map);
  return 0;
}

int init_img(int argc, char **argv, Config *cfg, ImageObjList *img_list){
  const char* folder_name;
  if (argc == 1) {
    folder_name = cfg->IMG_FOLDER;
  } else {
    folder_name = argv[1];
  }
  if(!folder_name){
    pprint(cfg,stderr,"Error memory allocation went wrong for the folder\n");
    return 1;
  }
  img_list->list = (ImageObj*)malloc(sizeof(ImageObj) * cfg->MAX_NUMBER_IMG);
  img_list->capacity = cfg->MAX_NUMBER_IMG;
  fetch_file(folder_name, img_list,cfg->IMG_W,cfg->IMG_H);
  return 0;
}

int init_raylib(Config *cfg){
  if(cfg->LOG_INFO)SetTraceLogLevel(LOG_NONE);
  if(cfg->ENABLE_FLAG)SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED);
  InitWindow(2 * cfg->RADIUS + 2 * cfg->OFFSET, 2 * cfg->RADIUS, cfg->W_TITLE);
  SetTargetFPS(60);
  return 0;
}


int main(int argc, char *argv[]){
  const char *file_path_config = "/home/sim/code/circleRay/config";
  int return_code = 0;

  Config cfg = {0};
  ImageObjList img_list = {0};

  if(init_config(&cfg, file_path_config))goto FREE;
  
  init_raylib(&cfg);
  init_img(argc,argv,&cfg,&img_list);
  

  Form *form_list = NULL;
  form_list = (Form*)malloc(sizeof(Form) * 11);
  if(!form_list){
    fprintf(stderr, "Couldn't allocate memory for form_list\n");
    return_code = 1;
    goto FREE;
  }
  int y_position = cfg.P;
  for(size_t index=0; index < 11; ++index){
    init_form(form_list + index,&cfg,index,&y_position);
  }


  View v = {0};
  v.begin = 0;
  v.nombre_img = img_list.current;

  Scroller scr = {0};
  scr.scroll_rate = cfg.SCROLL_RATE;
  scr.scroll_delay = cfg.SCROLL_DELAY;
  scr.current_index = &(v.begin);
  scr.next_scroll_time = 0.0;
  scr.number_img = img_list.current;

  reset_scroller(&scr);


  if (img_list.current == 0) {
      fprintf(stderr,"Aucune image trouvée dans le dossier : %s\n", cfg.IMG_FOLDER);
      return_code = 1;
      goto FREE;
  }

  while(!WindowShouldClose()){
    BeginDrawing();
      ClearBackground(BLANK);
      for(size_t i = 0; i < 11; ++i){
        draw_form(img_list,form_list + i,&cfg, i == 5);
      }
    EndDrawing();

    reset_scroller(&scr);
    
    if (IsKeyPressed(KEY_DOWN) | IsKeyPressed(KEY_J)) {
      press(&scr,&(scr.down));
    } 
    else if (IsKeyDown(KEY_DOWN) | IsKeyDown(KEY_J)) {
      scroll(&scr,&(scr.down));
    }

    if (IsKeyPressed(KEY_UP) | IsKeyPressed(KEY_K)) {
      press(&scr,&(scr.up));
    } 
    else if (IsKeyDown(KEY_UP) | IsKeyDown(KEY_K)) {
      scroll(&scr,&(scr.up));
    }

    if(IsKeyPressed(KEY_ENTER)){
      printf("%s\n",img_list.list[(v.begin + 5)%v.nombre_img].file_path);
      break;
    }
    
    move(&scr);
    set_index(form_list,&v);
  }

FREE:
  free(form_list);
  free(img_list.list);
  CloseWindow();
  return return_code;
}
