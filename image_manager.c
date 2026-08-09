#include "image_manager.h"
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>


static void load_img(ImageObj *img,int width, int height);

void fetch_file(const char* folder_path, ImageObjList *img_list,int width,int height) {
  DIR *dir;
  struct dirent *entry;
  struct stat statbuf;

  dir = opendir(folder_path);
  if (dir == NULL) {
      fprintf(stderr,"Couldn't open the folder : %s, this folder doesn't exist\n",folder_path);
      perror("opendir");
      return;
  }

  while ((entry = readdir(dir)) != NULL) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
          continue;
      }

      char fullpath[1024];
      snprintf(fullpath, sizeof(fullpath), "%s%s", folder_path, entry->d_name);
      
      if (stat(fullpath, &statbuf) == 0) {
          if (!S_ISDIR(statbuf.st_mode)) {
            if (img_list->current < img_list->capacity) {
              img_list->list[img_list->current].file_path = strdup(fullpath);
              load_img(&img_list->list[img_list->current],width,height);
              img_list->current++;
            }
          }
      }
  }

  closedir(dir);

}




void unload_img(ImageObj *img) {
  UnloadTexture(img->texture);
  if (img->file_path != NULL) {
    free((void*)img->file_path);
  }
}



void unload_img_list(ImageObjList img) {
  for (size_t i = 0 ; i < img.current; ++i) {
    unload_img(&img.list[i]);
  }
}

static void load_img(ImageObj *img,int width, int height) {
    const char *fileName = GetFileName(img->file_path); 
    
    char cache_path[1024];
    snprintf(cache_path, sizeof(cache_path), "/home/sim/.cache/wallpaper_thumbnail/wallpaper_cache_%s", fileName);

    if (FileExists(cache_path)) {
        img->texture = LoadTexture(cache_path); 
    } 
    else {
        Image temp = LoadImage(img->file_path);
        ImageResize(&temp, width,height);
        
        ExportImage(temp, cache_path);
        
        img->texture = LoadTextureFromImage(temp);
        UnloadImage(temp);
    }
}


