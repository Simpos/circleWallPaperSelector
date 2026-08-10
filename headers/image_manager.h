#ifndef IMAGE_MANAGER_H
#define IMAGE_MANAGER_H

#include "configParser.h"
#include "raylib.h"
#include <stddef.h>


typedef struct {
  const char* file_path;
  Texture2D texture;
  bool selected;
} ImageObj;

typedef struct {
  ImageObj *list;
  size_t capacity;
  size_t current;
} ImageObjList;


void fetch_file(const char* folder_path, ImageObjList *img_list,int width, int height);
void unload_img(ImageObj *img);
void unload_img_list(ImageObjList img);

#endif
