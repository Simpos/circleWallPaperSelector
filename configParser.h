#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H


#define MAX_LINE_LENGTH 100

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
  int RADIUS;
  int OFFSET;
  int P;
  long int BG_COLOR;
  long int FG_COLOR;
  long int BG_HOVER_COLOR;
  char *W_TITLE;
  size_t VIEW_COUNT;
  int FONT_SIZE;
  int TEXT_X_OFFSET;
  char *IMG_FOLDER;

  float SCROLL_DELAY; // 400ms avant que le défilement continu commence
  float SCROLL_RATE; // 80ms entre chaque saut d'image pendant le maintien


  int MAX_NUMBER_IMG;
  int IMG_W;
  int IMG_H;
  bool SILENT;
  bool LOG_INFO;

  bool ENABLE_FLAG;
} Config;


typedef enum {
  TYPE_INT,
  TYPE_STRING,
  TYPE_LONG_INT,
  TYPE_FLOAT
} ParamType;

typedef struct {
  const char *key;
  ParamType type;
  void *target_var;
} ConfigMapping;

int read_file(const char *file_path,Config *cfg,ConfigMapping *cfg_map);

// -- Config part --
void init_default_config(Config *cfg);
void print_config(Config *cfg);

#endif

#ifdef CONFIG_PARSER_IMPLEMENTATION

// ---------------------------- Read and parse config file --------------------------
//
void pprint(Config *cfg,FILE *stream,const char *fmt, ...) {
  if(cfg->SILENT) return;
  va_list args;
  va_start(args, fmt);
  vfprintf(stream,fmt, args);
  va_end(args);
}

static char* trim(char *str) {
    while (isspace((unsigned char)*str)) {
        str++;
    }
    if (*str == '\0') return str; // Chaîne vide
    
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) {
        end--;
    }
    *(end + 1) = '\0';
    
    return str;
}

static void update_variable(const char *key, const char *value, Config *cfg, ConfigMapping *mappings) {
    for (int i = 0; mappings[i].key != NULL; i++) {
        
        if (strcmp(key, mappings[i].key) == 0) {
            
            // On a trouvé la clé, on agit selon son TYPE
            switch (mappings[i].type) {
                
                case TYPE_INT: {
                    // On convertit le pointeur void* en int*, puis on modifie la valeur pointée
                    int *target = (int *)mappings[i].target_var;
                    char *endptr;
                    long val = strtol(value, &endptr, 0);
                    if(endptr == value || *endptr != '\0'){
                      pprint(cfg,stderr,"Error : invalid value for '%s' : '%s'", key, value);
                      return;
                    }
                    *target = (int)val;
                    pprint(cfg,stderr,"Mis à jour (INT) : %s = %d\n", key, *target);
                    break;
                }
                case TYPE_LONG_INT : {
                    long int *target = (long int *)mappings[i].target_var;
                    char *endptr;
                    long val = strtol(value, &endptr, 16);
                    if(endptr == value || *endptr != '\0'){
                      pprint(cfg,stderr,"Error : invalid value for '%s' : '%s'", key, value);
                      return;
                    }
                    *target = val;
                    pprint(cfg,stderr,"Mis à jour (LONG_INT) : %s = %ld = %X\n", key, *target, *target);
                    break;
                }

                case TYPE_FLOAT : {
                    float *target = (float *)mappings[i].target_var;
                    char *endptr;
                    float val = strtof(value, &endptr);
                    if(endptr == value || *endptr != '\0'){
                      pprint(cfg,stderr,"Error : invalid value for '%s' : '%s'", key, value);
                      return;
                    }
                    *target = val;
                    pprint(cfg,stderr,"Mis à jour (FLOAT) : %s = %f\n", key, *target);
                    break;
                    
                }
                
                case TYPE_STRING: {
                    char **target = (char **)mappings[i].target_var;
                    char *new_val =strdup(value); 

                    if(new_val == NULL){
                      pprint(cfg,stderr, "Error, invalid memory allocation\n");
                      return;
                    }
                    if (*target != NULL) {
                        free(*target);
                    }
                    *target = new_val;
                    pprint(cfg,stderr,"Mis à jour (STRING) : %s = \"%s\"\n", key, *target);
                    break;
                }
            }
            return;
        }
    }
    pprint(cfg,stderr,"Avertissement : Option '%s' inconnue.\n", key);
}

static void parse_line(char *line,Config *cfg, ConfigMapping *cfg_map){
    if (line[0] == '\0' || line[0] == '\n') return;

    char *comment = strchr(line, '#');
    if (comment != NULL) {
        *comment = '\0'; 
    }

    char *equals = strchr(line, '=');
    if (equals == NULL) {
        return; // Pas de '=' trouvé, on ignore la ligne
    }

    *equals = '\0';

    char *key = trim(line);
    char *value = trim(equals + 1); // La valeur commence juste après le '='

    if (strlen(key) > 0) {
        update_variable(key, value, cfg, cfg_map);
    }
}

int read_file(const char *file_path,Config *cfg, ConfigMapping *cfg_map){
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        pprint(cfg,stderr,"Erreur d'ouverture du fichier de configuration");
        return 0; 
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        parse_line(line,cfg,cfg_map);
    }

    fclose(file);
    return 1;
}

// --------------- Config part -------------------
void init_default_config(Config *cfg){
  cfg->RADIUS = 200;
  cfg->IMG_FOLDER = strdup("/home/sim/Images/wallpaper/");
  cfg->P = (cfg->RADIUS * 10) / 400;
}

#endif
